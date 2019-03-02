#ifndef ETH_API_H
#define ETH_API_H

#include "../core/client/client.h"
#include "../core/util/utils.h"

typedef struct {
  uint8_t data[32];
} uint256_t;

/** a transaction */
typedef struct eth_tx {
  bytes32_t hash;              /**< the blockhash */
  bytes32_t block_hash;        /**< hash of ther containnig block */
  u_int64_t block_number;      /**< number of the containing block */
  address_t from;              /**< sender of the tx */
  u_int64_t gas;               /**< gas send along */
  u_int64_t gas_price;         /**< gas price used */
  bytes_t   data;              /**< data send along with the transaction */
  u_int64_t nonce;             /**< nonce of the transaction */
  address_t to;                /**< receiver of the address 0x0000.. -Address is used for contract creation. */
  uint256_t value;             /**< the value in wei send */
  int       transaction_index; /**< the transaction index */
  uint8_t   signature[65];     /**< signature of the transaction */
} eth_tx_t;

/** a Ethereum Block */
typedef struct eth_block {
  uint64_t   number;            /**< the blockNumber */
  bytes32_t  hash;              /**< the blockhash */
  uint64_t   gasUsed;           /**< gas used by all the transactions */
  uint64_t   gasLimit;          /**< gasLimit */
  address_t  author;            /**< the author of the block. */
  uint256_t  difficulty;        /**< the difficulty of the block. */
  bytes_t    extra_data;        /**< the extra_data of the block. */
  uint8_t    logsBloom[256];    /**< the logsBloom-data */
  bytes32_t  parent_hash;       /**< the hash of the parent-block */
  bytes32_t  sha3_uncles;       /**< root hash of the uncle-trie*/
  bytes32_t  state_root;        /**< root hash of the state-trie*/
  bytes32_t  receipts_root;     /**< root of the receipts trie */
  bytes32_t  transaction_root;  /**< root of the transaction trie */
  int        tx_count;          /**< number of transactions in the block */
  eth_tx_t*  tx_data;           /**< array of transaction data or NULL if not requested */
  bytes32_t* tx_hashes;         /**< array of transaction hashes or NULL if not requested */
  uint64_t   timestamp;         /**< the unix timestamp of the block */
  bytes_t*   seal_fields;       /**< sealed fields */
  int        seal_fields_count; /**< number of seal fields */

  /* data */
} eth_block_t;

uint256_t    eth_getBalance(in3_t* in3, address_t account, uint64_t block);      /**< returns the balance of the account of given address. */
uint64_t     eth_blockNumber(in3_t* in3);                                        /**< returns the current price per gas in wei. */
uint64_t     eth_gasPrice(in3_t* in3);                                           /**< returns the current blockNumber, if bn==0 an error occured and you should check eth_last_error() */
eth_block_t* eth_getBlockByNumber(in3_t* in3, uint64_t number, bool include_tx); /**< returns the block for the given number (if number==0, the latest will be returned). If result is null, check eth_last_error()! otherwise make sure to free the result after using it! */
eth_block_t* eth_getBlockByHash(in3_t* in3, bytes32_t hash, bool include_tx);    /**< returns the block for the given hash. If result is null, check eth_last_error()! otherwise make sure to free the result after using it! */

char*       eth_last_error(); /**< the current error or null if all is ok */
long double as_double(uint256_t d);
uint64_t    as_long(uint256_t d);
#endif