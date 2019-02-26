
#include "../core/client/verifier.h"
#include "../core/util/bytes.h"
#ifndef evm_h__
#define evm_h__

//#define EVM_GAS

typedef enum evm_state {
  EVM_STATE_INIT     = 0,
  EVM_STATE_RUNNING  = 1,
  EVM_STATE_STOPPED  = 2,
  EVM_STATE_REVERTED = 3
} evm_state_t;

#define EVM_ERROR_EMPTY_STACK -1
#define EVM_ERROR_INVALID_OPCODE -2
#define EVM_ERROR_BUFFER_TOO_SMALL -3
#define EVM_ERROR_ILLEGAL_MEMORY_ACCESS -4
#define EVM_ERROR_INVALID_JUMPDEST -5
#define EVM_ERROR_INVALID_PUSH -6
#define EVM_ERROR_UNSUPPORTED_CALL_OPCODE -7
#define EVM_ERROR_TIMEOUT -8
#define EVM_ERROR_INVALID_ENV -9
#define EVM_ERROR_OUT_OF_GAS -10
#define EVM_ERROR_BALANCE_TOO_LOW -11
#define EVM_ERROR_STACK_LIMIT -12

#define EVM_PROP_FRONTIER 1
#define EVM_PROP_EIP150 2
#define EVM_PROP_EIP158 4
#define EVM_PROP_CONSTANTINOPL 16
#define EVM_PROP_NO_FINALIZE 32768
#define EVM_PROP_DEBUG 65536
#define EVM_PROP_STATIC 256

#define EVM_ENV_BALANCE 1
#define EVM_ENV_CODE_SIZE 2
#define EVM_ENV_CODE_COPY 3
#define EVM_ENV_BLOCKHASH 4
#define EVM_ENV_STORAGE 5
#define EVM_ENV_BLOCKHEADER 6
#define EVM_ENV_CODE_HASH 7
#define EVM_ENV_NONCE 8

/**
 * This function provides data from the enviroment.
 * 
 * depending on the key the function will set the out_data-pointer to the result. 
 * This means the enviroment is responsible for memory management and also to clean up resources afterwards.
 * 
 * @param evm passes the current evm-pointer
 * @param evm_key the requested value
 * @param in_data the pointer to the argument like the address
 * @param  
 * 
 * 
 */
typedef int (*evm_get_env)(void* evm, uint16_t evm_key, uint8_t* in_data, int in_len, uint8_t** out_data, int offset, int len);

typedef struct account_storage {
  bytes32_t               key;
  bytes32_t               value;
  struct account_storage* next;
} storage_t;
typedef struct logs {
  bytes_t      topics;
  bytes_t      data;
  struct logs* next;
} logs_t;

typedef struct account {
  address_t       address;
  bytes32_t       balance;
  bytes32_t       nonce;
  bytes_t         code;
  storage_t*      storage;
  struct account* next;
} account_t;

typedef struct evm {
  // internal data
  bytes_builder_t stack;
  //  bytes_builder_t memory;
  bytes_builder_t memory;
  int             stack_size;
  bytes_t         code;
  uint32_t        pos;
  evm_state_t     state;
  bytes_t         last_returned;
  bytes_t         return_data;
  uint32_t*       invalid_jumpdest;

  // set properties as to which EIPs to use.
  uint32_t properties;

  // define the enviroment-function.
  evm_get_env env;
  void*       env_ptr;

  //
  uint8_t* address;
  uint8_t* account;
  uint8_t* origin;
  uint8_t* caller;
  bytes_t  call_value;
  bytes_t  call_data;
  bytes_t  gas_price;

#ifdef EVM_GAS
  uint64_t    gas;
  account_t*  accounts;
  struct evm* parent;
  logs_t*     logs;
  uint64_t    refund;
  uint64_t    init_gas;

#endif

} evm_t;

int evm_stack_push(evm_t* evm, uint8_t* data, uint8_t len);
int evm_stack_push_ref(evm_t* evm, uint8_t** dst, uint8_t len);
int evm_stack_push_int(evm_t* evm, uint32_t val);
int evm_stack_push_long(evm_t* evm, uint64_t val);

int     evm_stack_get_ref(evm_t* evm, uint8_t pos, uint8_t** dst);
int     evm_stack_pop(evm_t* evm, uint8_t* dst, uint8_t len);
int     evm_stack_pop_ref(evm_t* evm, uint8_t** dst);
int     evm_stack_pop_byte(evm_t* evm, uint8_t* dst);
int32_t evm_stack_pop_int(evm_t* evm);
int     evm_stack_peek_len(evm_t* evm);

int evm_run(evm_t* evm);
#define EVM_CALL_MODE_STATIC 1
#define EVM_CALL_MODE_DELEGATE 2

int evm_sub_call(evm_t*   parent,
                 uint8_t  address[20],
                 uint8_t  account[20],
                 uint8_t* value, wlen_t l_value,
                 uint8_t* data, uint32_t l_data,
                 uint8_t  caller[20],
                 uint8_t  origin[20],
                 uint64_t gas,
                 wlen_t   mode,
                 uint32_t out_offset, uint32_t out_len);

int     evm_ensure_memory(evm_t* evm, uint32_t max_pos);
int     in3_get_env(void* evm_ptr, uint16_t evm_key, uint8_t* in_data, int in_len, uint8_t** out_data, int offset, int len);
int     evm_call(in3_vctx_t* vc,
                 uint8_t     address[20],
                 uint8_t* value, wlen_t l_value,
                 uint8_t* data, uint32_t l_data,
                 uint8_t   caller[20],
                 uint64_t  gas,
                 bytes_t** result);
void    evm_print_stack(evm_t* evm, uint64_t last_gas, uint32_t pos);
void    evm_free(evm_t* evm);
int     evm_run_precompiled(evm_t* evm, uint8_t address[20]);
uint8_t evm_is_precompiled(evm_t* evm, uint8_t address[20]);
void    uint256_set(uint8_t* src, wlen_t src_len, uint8_t dst[32]);

#ifdef EVM_GAS
account_t* evm_get_account(evm_t* evm, uint8_t adr[20], wlen_t create);
storage_t* evm_get_storage(evm_t* evm, uint8_t adr[20], uint8_t* key, wlen_t keylen, wlen_t create);
int        transfer_value(evm_t* evm, uint8_t from_account[20], uint8_t to_account[20], uint8_t* value, wlen_t value_len, uint32_t base_gas);

#endif
#endif