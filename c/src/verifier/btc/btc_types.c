#include "btc_types.h"
#include "../../core/util/mem.h"
#include "btc_serialize.h"

// Transaction fixed size values
#define BTC_TX_VERSION_SIZE_BYTES  4
#define BTC_TX_LOCKTIME_SIZE_BYTES 4

// Input fixed size values
#define BTC_TX_IN_PREV_OUPUT_SIZE_BYTES 36 // Outpoint = prev txid (32 bytes) + output index (4 bytes)
#define BTC_TX_IN_SEQUENCE_SIZE_BYTES   4

// Output fixed size values
#define BTC_TX_OUT_VALUE_SIZE_BYTES 8

uint8_t* btc_parse_tx_in(uint8_t* data, btc_tx_in_t* dst, uint8_t* limit) {
  uint64_t len;
  dst->prev_tx_hash  = data;
  dst->prev_tx_index = le_to_int(data + 32);
  dst->script.data   = data + 36 + decode_var_int(data + 36, &len);
  dst->script.len    = (uint32_t) len;
  if (dst->script.data + dst->script.len + 4 > limit) return NULL; // check limit
  dst->sequence = le_to_int(dst->script.data + dst->script.len);
  return dst->script.data + dst->script.len + 4;
}

// WARNING: You need to free 'dst' pointer after calling this function
// TODO: Implement support for "Coinbase" inputs
// TODO: Handle null arguments
// TODO: Handle max script len = 10000 bytes
void btc_serialize_tx_in(btc_tx_in_t* tx_in, bytes_t* dst) {
  if (!tx_in || !dst) return;
  // calculate serialized tx input size in bytes
  uint32_t tx_in_size = (BTC_TX_IN_PREV_OUPUT_SIZE_BYTES +
                         get_compact_uint_size((uint64_t) tx_in->script.len) +
                         tx_in->script.len +
                         BTC_TX_IN_SEQUENCE_SIZE_BYTES);

  // alloc memory in dst
  dst->data = malloc(tx_in_size * sizeof(*dst->data));
  dst->len  = tx_in_size;

  // serialize tx_in
  // -- Previous outpoint
  if (!tx_in->prev_tx_hash) return;
  uint32_t index = 0;
  for (uint32_t i = 0; i < 32; i++) {
    dst->data[index++] = tx_in->prev_tx_hash[i];
  }
  uint_to_le(dst, index, tx_in->prev_tx_index);
  index += 4;

  // -- script
  long_to_compact_uint(dst, index, tx_in->script.len);
  index += get_compact_uint_size(tx_in->script.len);

  for (uint32_t i = 0; i < tx_in->script.len; i++) {
    dst->data[index++] = tx_in->script.data[i];
  }

  // -- sequence
  uint_to_le(dst, index, tx_in->sequence);
}

uint8_t* btc_parse_tx_out(uint8_t* data, btc_tx_out_t* dst) {
  uint64_t len;
  dst->value       = le_to_long(data);
  dst->script.data = data + 8 + decode_var_int(data + 8, &len);
  dst->script.len  = (uint32_t) len;
  return dst->script.data + dst->script.len;
}

// WARNING: You need to free 'dst' pointer after calling this function
// TODO: Handle null arguments
// TODO: Handle max script len = 10000 bytes
void btc_serialize_tx_out(btc_tx_out_t* tx_out, bytes_t* dst) {
  // calculate serialized tx output size in bytes
  uint32_t tx_out_size = (BTC_TX_OUT_VALUE_SIZE_BYTES +
                          get_compact_uint_size((uint64_t) tx_out->script.len) +
                          tx_out->script.len);

  // alloc memory in dst
  dst->data = malloc(tx_out_size * sizeof(*dst->data));
  dst->len  = tx_out_size;

  // serialize tx_out
  uint32_t index = 0;

  // -- value
  long_to_le(dst, index, tx_out->value);
  index += 8;

  // -- pk_script size
  long_to_compact_uint(dst, index, tx_out->script.len);
  index += get_compact_uint_size((uint64_t) tx_out->script.len);

  // -- pk_script
  for (uint32_t i = 0; i < tx_out->script.len; i++) {
    dst->data[index++] = tx_out->script.data[i];
  }
}

in3_ret_t btc_parse_tx(bytes_t tx, btc_tx_t* dst) {
  uint64_t     val;
  btc_tx_in_t  tx_in;
  btc_tx_out_t tx_out;
  dst->all     = tx;
  dst->version = le_to_int(tx.data);
  dst->flag    = btc_is_witness(tx) ? 1 : 0;
  uint8_t* end = tx.data + tx.len;
  uint8_t* p   = tx.data + (dst->flag ? 6 : 4);

  p += decode_var_int(p, &val);
  if (p >= end) return IN3_EINVAL;
  dst->input_count = (uint32_t) val;
  dst->input.data  = p;
  for (uint32_t i = 0; i < dst->input_count; i++) {
    p = btc_parse_tx_in(p, &tx_in, end);
    if (!p || p >= end) return IN3_EINVAL;
  }
  dst->input.len = p - dst->input.data;

  p += decode_var_int(p, &val);
  dst->output_count = (uint32_t) val;
  dst->output.data  = p;
  for (uint32_t i = 0; i < dst->output_count; i++) {
    p = btc_parse_tx_out(p, &tx_out);
    if (p > end) return IN3_EINVAL;
  }
  dst->output.len = p - dst->output.data;
  dst->witnesses  = bytes(p, tx.data + tx.len - 4 - p);
  dst->lock_time  = le_to_int(tx.data + tx.len - 4);

  return IN3_OK;
}

// Converts a btc transaction into a serialized transaction
// WARNING: You need to free dst pointer after using this function!
// TODO: Error handling for null tx and dst pointers
in3_ret_t btc_serialize_tx(btc_tx_t* tx, bytes_t* dst) {
  // Clean exit buffer
  dst->len = 0;

  // calculate transaction size in bytes
  uint32_t tx_size;
  tx_size = (BTC_TX_VERSION_SIZE_BYTES +
             (2 * tx->flag) +
             get_compact_uint_size((uint64_t) tx->input_count) +
             tx->input.len +
             get_compact_uint_size((uint64_t) tx->output_count) +
             tx->output.len +
             tx->witnesses.len +
             BTC_TX_LOCKTIME_SIZE_BYTES);

  dst->data = malloc(tx_size * sizeof(*dst->data));
  dst->len  = tx_size;

  // Serialize transaction data
  uint32_t index = 0;
  // version
  uint_to_le(dst, index, tx->version);
  index += 4;
  // Check if transaction uses SegWit
  if (tx->flag) {
    dst->data[index++] = 0;
    dst->data[index++] = 1;
  }
  // input_count
  long_to_compact_uint(dst, index, tx->input_count);
  index += get_compact_uint_size(tx->input_count);
  // inputs
  // TODO: serialize struct if tx_in is not null
  for (uint32_t i = 0; i < tx->input.len; i++) {
    dst->data[index++] = tx->input.data[i];
  }
  // output_count
  long_to_compact_uint(dst, index, tx->output_count);
  index += get_compact_uint_size(tx->output_count);
  // outputs
  // TODO: serialize struct if tx_out is not null
  for (uint32_t i = 0; i < tx->output.len; i++) {
    dst->data[index++] = tx->output.data[i];
  }
  // Include witness
  if (tx->flag) {
    for (uint32_t i = 0; i < tx->witnesses.len; i++) {
      dst->data[index++] = tx->witnesses.data[i];
    }
  }
  // locktime
  //uint_to_le(dst, index, tx->lock_time);
  dst->data[index + 3] = ((tx->lock_time >> 24) & 0xff);
  dst->data[index + 2] = ((tx->lock_time >> 16) & 0xff);
  dst->data[index + 1] = ((tx->lock_time >> 8) & 0xff);
  dst->data[index]     = ((tx->lock_time) & 0xff);

  return IN3_OK;
}

uint32_t btc_vsize(btc_tx_t* tx) {
  uint32_t w = btc_weight(tx);
  return w % 4 ? (w + 4) / 4 : w / 4;
}

uint32_t btc_weight(btc_tx_t* tx) {
  const uint32_t w = tx->witnesses.len
                         ? (tx->all.len - tx->witnesses.len - 2) * 3 + tx->all.len
                         : tx->all.len * 4;
  return w;
}

in3_ret_t btc_tx_id(btc_tx_t* tx, bytes32_t dst) {
  bytes_t  data;
  uint8_t* start = tx->all.data + (tx->flag ? 6 : 4);
  data.len       = tx->output.len + tx->output.data - start + 8;
  data.data      = data.len > 1000 ? _malloc(data.len) : alloca(data.len);
  memcpy(data.data, tx->all.data, 4);                                  // nVersion
  memcpy(data.data + 4, start, data.len - 8);                          // txins/txouts
  memcpy(data.data + data.len - 4, tx->all.data + tx->all.len - 4, 4); // lockTime

  btc_hash(data, dst);
  if (data.len > 1000) _free(data.data);
  return IN3_OK;
}

// creates a raw unsigned transaction
// TODO: implement better error handling
// TODO: Support witnesses
void create_raw_tx(btc_tx_in_t* tx_in, uint32_t tx_in_len, btc_tx_out_t* tx_out, uint32_t tx_out_len, uint32_t lock_time, bytes_t* dst_raw_tx) {
  if (!tx_in || !tx_out || !dst_raw_tx || tx_in_len == 0 || tx_out_len == 0) {
    // TODO: Implement better error handling
    printf("ERROR: arguments for creating a btc transaction can not be null\n");
    return;
  }
  btc_tx_t tx;
  tx.version      = 1;
  tx.flag         = 0;
  tx.input_count  = tx_in_len;
  tx.output_count = tx_out_len;
  tx.lock_time    = lock_time;

  // Get inputs
  // -- serialize inputs
  bytes_t* serialized_inputs = malloc(tx_in_len * sizeof(bytes_t));
  uint32_t raw_input_size    = 0;
  for (uint32_t i = 0; i < tx_in_len; i++) {
    btc_serialize_tx_in(&tx_in[i], &serialized_inputs[i]);
    raw_input_size += serialized_inputs[i].len;
  }
  // -- Copy raw inputs into tx
  tx.input.data           = malloc(raw_input_size);
  tx.input.len            = raw_input_size;
  uint32_t prev_input_len = 0;
  for (uint32_t i = 0; i < tx_in_len; i++) {
    for (uint32_t j = 0; j < serialized_inputs[i].len; j++) {
      tx.input.data[j + prev_input_len] = serialized_inputs[i].data[j];
    }
    prev_input_len = serialized_inputs[i].len;
  }

  // Get Outputs
  // -- serialize outputs
  bytes_t* serialized_outputs = malloc(tx_out_len * sizeof(bytes_t));
  uint32_t raw_output_size    = 0;
  for (uint32_t i = 0; i < tx_out_len; i++) {
    btc_serialize_tx_out(&tx_out[i], &serialized_outputs[i]);
    raw_output_size += serialized_outputs[i].len;
  }
  // -- Copy raw outputs into tx
  tx.output.data           = malloc(raw_output_size);
  tx.output.len            = raw_output_size;
  uint32_t prev_output_len = 0;
  for (uint32_t i = 0; i < tx_out_len; i++) {
    for (uint32_t j = 0; j < serialized_outputs[i].len; j++) {
      tx.output.data[j + prev_output_len] = serialized_outputs[i].data[j];
    }
    prev_output_len = serialized_outputs[i].len;
  }

  // free buffers
  for (uint32_t i = 0; i < tx_in_len; i++) {
    _free(serialized_inputs[i].data);
  }
  _free(serialized_inputs);
  for (uint32_t i = 0; i < tx_out_len; i++) {
    _free(serialized_outputs[i].data);
  }
  _free(serialized_outputs);
}