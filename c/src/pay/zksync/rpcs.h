/*******************************************************************************
 * This file is part of the Incubed project.
 * Sources: https://github.com/slockit/in3-c
 *
 * Copyright (C) 2018-2022 slock.it GmbH, Blockchains LLC
 *
 *
 * COMMERCIAL LICENSE USAGE
 *
 * Licensees holding a valid commercial license may use this file in accordance
 * with the commercial license agreement provided with the Software or, alternatively,
 * in accordance with the terms contained in a written agreement between you and
 * slock.it GmbH/Blockchains LLC. For licensing terms and conditions or further
 * information please contact slock.it at in3@slock.it.
 *
 * Alternatively, this file may be used under the AGPL license as follows:
 *
 * AGPL LICENSE USAGE
 *
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Affero General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
 * [Permissions of this strong copyleft license are conditioned on making available
 * complete source code of licensed works and modifications, which include larger
 * works using a licensed work, under the same license. Copyright and license notices
 * must be preserved. Contributors provide an express grant of patent rights.]
 * You should have received a copy of the GNU Affero General Public License along
 * with this program. If not, see <https://www.gnu.org/licenses/>.
 *******************************************************************************/

// ::: This is a autogenerated file. Do not edit it manually! :::

#include "../../core/client/request_internal.h"

// list of availbale rpc-functions
#ifndef __RPC_ZKSYNC_H
#define __RPC_ZKSYNC_H

/**
 * returns the contract address
 *
 * Returns:
 *   - d_token_t* : ([object Object]) fetches the contract addresses from the zksync server. This request also caches them and will return the results from cahe if available.
 */
static inline in3_ret_t rpc_call_zksync_contract_address(in3_rpc_handle_ctx_t* ctx, d_token_t** res) {
  in3_ret_t r = req_send_sub_request(ctx->req, "zksync_contract_address", "", NULL, res, NULL);
  return r;
}
#define FN_ZKSYNC_CONTRACT_ADDRESS "zksync_contract_address"

/**
 * returns the list of all available tokens
 *
 * Returns:
 *   - d_token_t* : ([object Object]) a array of tokens-definitions. This request also caches them and will return the results from cahe if available.
 */
static inline in3_ret_t rpc_call_zksync_tokens(in3_rpc_handle_ctx_t* ctx, d_token_t** res) {
  in3_ret_t r = req_send_sub_request(ctx->req, "zksync_tokens", "", NULL, res, NULL);
  return r;
}
#define FN_ZKSYNC_TOKENS "zksync_tokens"

/**
 * returns account_info from the server
 *
 *
 * Parameters:
 *
 *   - uint8_t* address : (address) the account-address. if not specified, the client will try to use its own address based on the signer config.
 * Returns:
 *   - d_token_t* : ([object Object]) the current state of the requested account.
 */
static inline in3_ret_t rpc_call_zksync_account_info(in3_rpc_handle_ctx_t* ctx, d_token_t** res, uint8_t* address) {
  char*     jpayload = sprintx("\"%B\"", bytes(address, 20));
  in3_ret_t r        = req_send_sub_request(ctx->req, "zksync_account_info", jpayload, NULL, res, NULL);
  _free(jpayload);
  return r;
}
#define FN_ZKSYNC_ACCOUNT_INFO "zksync_account_info"

/**
 * returns the state or receipt of the the zksync-tx
 *
 *
 * Parameters:
 *
 *   - bytes_t tx : (bytes32) the txHash of the send tx
 * Returns:
 *   - d_token_t* : ([object Object]) the current state of the requested tx.
 */
static inline in3_ret_t rpc_call_zksync_tx_info(in3_rpc_handle_ctx_t* ctx, d_token_t** res, bytes_t tx) {
  char*     jpayload = sprintx("\"%B\"", (bytes_t) tx);
  in3_ret_t r        = req_send_sub_request(ctx->req, "zksync_tx_info", jpayload, NULL, res, NULL);
  _free(jpayload);
  return r;
}
#define FN_ZKSYNC_TX_INFO "zksync_tx_info"

/**
 * returns the full input data of a transaction. In order to use this, the `rest_api` needs to be set in the config.
 *
 *
 * Parameters:
 *
 *   - bytes_t tx : (bytes32) the txHash of the send tx
 * Returns:
 *   - d_token_t* : ([object Object]) the data and state of the requested tx.
 */
static inline in3_ret_t rpc_call_zksync_tx_data(in3_rpc_handle_ctx_t* ctx, d_token_t** res, bytes_t tx) {
  char*     jpayload = sprintx("\"%B\"", (bytes_t) tx);
  in3_ret_t r        = req_send_sub_request(ctx->req, "zksync_tx_data", jpayload, NULL, res, NULL);
  _free(jpayload);
  return r;
}
#define FN_ZKSYNC_TX_DATA "zksync_tx_data"

/**
 * returns the history of transaction for a given account.
 *
 *
 * Parameters:
 *
 *   - uint8_t* account   : (address) the address of the account
 *   - char*    ref_start : (string) the reference or start. this could be a tx_id prefixed with `<` or `>`for newer or older than the specified  tx or `pending` returning all pending tx.
 *   - bytes_t  limit     : (int) the max number of entries to return
 * Returns:
 *   - d_token_t* : (zk_history) the data and state of the requested tx.
 */
static inline in3_ret_t rpc_call_zksync_account_history(in3_rpc_handle_ctx_t* ctx, d_token_t** res, uint8_t* account, char* ref_start, bytes_t limit) {
  char*     jpayload = sprintx("\"%B\",\"%S\",\"%B\"", bytes(account, 20), (char*) ref_start, (bytes_t) limit);
  in3_ret_t r        = req_send_sub_request(ctx->req, "zksync_account_history", jpayload, NULL, res, NULL);
  _free(jpayload);
  return r;
}
#define FN_ZKSYNC_ACCOUNT_HISTORY "zksync_account_history"

/**
 * sets the signerkey based on the current pk or as configured in the config.
 * You can specify the key by either
 * - setting a signer ( the sync key will be derrived through a signature )
 * - setting the seed directly ( `sync_key` in the config)
 * - setting the `musig_pub_keys` to generate the pubKeyHash based on them
 * - setting the `create2` options and the sync-key will generate the account based on the pubKeyHash
 *
 *
 * we support 3 different signer types (`signer_type` in the `zksync` config) :
 *
 * 1. `pk` - Simple Private Key
 *     If a signer is set (for example by setting the pk), incubed will derrive the sync-key through a signature and use it
 * 2. `contract` - Contract Signature
 *     In this case a preAuth-tx will be send on L1 using the signer. If this contract is a mutisig, you should make sure, you have set the account explicitly in the config and also activate the multisig-plugin, so the transaction will be send through the multisig.
 * 3. `create2` - Create2 based Contract
 *
 *
 *
 * Parameters:
 *
 *   - char* token : (string) the token to pay the gas (either the symbol or the address)
 * Returns:
 *   - uint8_t* : (address) the pubKeyHash, if it was executed successfully
 */
static inline in3_ret_t rpc_call_zksync_set_key(in3_rpc_handle_ctx_t* ctx, uint8_t** _res, char* token) {
  d_token_t* res      = NULL;
  char*      jpayload = sprintx("\"%S\"", (char*) token);
  in3_ret_t  r        = req_send_sub_request(ctx->req, "zksync_set_key", jpayload, NULL, &res, NULL);
  _free(jpayload);
  if (!r) *_res = d_bytes(res).data;
  return r;
}
#define FN_ZKSYNC_SET_KEY "zksync_set_key"

/**
 * returns the current PubKeyHash based on the configuration set.
 *
 *
 * Parameters:
 *
 *   - bytes_t pubKey : (bytes32) the packed public key to hash ( if given the hash is build based on the given hash, otherwise the hash is based on the config)
 * Returns:
 *   - uint8_t* : (address) the pubKeyHash
 */
static inline in3_ret_t rpc_call_zksync_pubkeyhash(in3_rpc_handle_ctx_t* ctx, uint8_t** _res, bytes_t pubKey) {
  d_token_t* res      = NULL;
  char*      jpayload = sprintx("\"%B\"", (bytes_t) pubKey);
  in3_ret_t  r        = req_send_sub_request(ctx->req, "zksync_pubkeyhash", jpayload, NULL, &res, NULL);
  _free(jpayload);
  if (!r) *_res = d_bytes(res).data;
  return r;
}
#define FN_ZKSYNC_PUBKEYHASH "zksync_pubkeyhash"

/**
 * returns the current packed PubKey based on the config set.
 *
 * If the config contains public keys for musig-signatures, the keys will be aggregated, otherwise the pubkey will be derrived from the signing key set.
 *
 *
 * Returns:
 *   - bytes_t : (bytes32) the pubKey
 */
static inline in3_ret_t rpc_call_zksync_pubkey(in3_rpc_handle_ctx_t* ctx, bytes_t* _res) {
  d_token_t* res = NULL;
  in3_ret_t  r   = req_send_sub_request(ctx->req, "zksync_pubkey", "", NULL, &res, NULL);
  if (!r) *_res = d_bytes(res);
  return r;
}
#define FN_ZKSYNC_PUBKEY "zksync_pubkey"

/**
 * returns the address of the account used.
 *
 * Returns:
 *   - uint8_t* : (address) the account used.
 */
static inline in3_ret_t rpc_call_zksync_account_address(in3_rpc_handle_ctx_t* ctx, uint8_t** _res) {
  d_token_t* res = NULL;
  in3_ret_t  r   = req_send_sub_request(ctx->req, "zksync_account_address", "", NULL, &res, NULL);
  if (!r) *_res = d_bytes(res).data;
  return r;
}
#define FN_ZKSYNC_ACCOUNT_ADDRESS "zksync_account_address"

/**
 * returns the schnorr musig signature based on the current config.
 *
 * This also supports signing with multiple keys. In this case the configuration needs to sets the urls of the other keys, so the client can then excange all data needed in order to create the combined signature.
 * when exchanging the data with other keys, all known data will be send using `zk_sign` as method, but instead of the raw message a object with those data will be passed.
 *
 *
 *
 * Parameters:
 *
 *   - bytes_t message : (bytes) the message to sign
 * Returns:
 *   - bytes_t : (bytes96) The return value are 96 bytes of signature:
 *         - `[0...32]` packed public key
 *         - `[32..64]` r-value
 *         - `[64..96]` s-value
 */
static inline in3_ret_t rpc_call_zksync_sign(in3_rpc_handle_ctx_t* ctx, bytes_t* _res, bytes_t message) {
  d_token_t* res      = NULL;
  char*      jpayload = sprintx("\"%B\"", (bytes_t) message);
  in3_ret_t  r        = req_send_sub_request(ctx->req, "zksync_sign", jpayload, NULL, &res, NULL);
  _free(jpayload);
  if (!r) *_res = d_bytes(res);
  return r;
}
#define FN_ZKSYNC_SIGN "zksync_sign"

/**
 * returns 0 or 1 depending on the successfull verification of the signature.
 *
 * if the `musig_pubkeys` are set it will also verify against the given public keys list.
 *
 *
 *
 * Parameters:
 *
 *   - bytes_t message   : (bytes) the message which was supposed to be signed
 *   - bytes_t signature : (bytes96) the signature (96 bytes)
 * Returns:
 *   - bytes_t : (int) 1 if the signature(which contains the pubkey as the first 32bytes) matches the message.
 */
static inline in3_ret_t rpc_call_zksync_verify(in3_rpc_handle_ctx_t* ctx, bytes_t* _res, bytes_t message, bytes_t signature) {
  d_token_t* res      = NULL;
  char*      jpayload = sprintx("\"%B\",\"%B\"", (bytes_t) message, (bytes_t) signature);
  in3_ret_t  r        = req_send_sub_request(ctx->req, "zksync_verify", jpayload, NULL, &res, NULL);
  _free(jpayload);
  if (!r) *_res = d_bytes(res);
  return r;
}
#define FN_ZKSYNC_VERIFY "zksync_verify"

/**
 * returns the state or receipt of the the PriorityOperation
 *
 *
 * Parameters:
 *
 *   - uint64_t opId : (uint64) the opId of a layer-operstion (like depositing)
 * Returns:
 *   - d_token_t* : ([object Object]) state of the PriorityOperation
 */
static inline in3_ret_t rpc_call_zksync_ethop_info(in3_rpc_handle_ctx_t* ctx, d_token_t** res, uint64_t opId) {
  char*     jpayload = sprintx("\"%U\"", (uint64_t) opId);
  in3_ret_t r        = req_send_sub_request(ctx->req, "zksync_ethop_info", jpayload, NULL, res, NULL);
  _free(jpayload);
  return r;
}
#define FN_ZKSYNC_ETHOP_INFO "zksync_ethop_info"

/**
 * returns current token-price
 *
 *
 * Parameters:
 *
 *   - char* token : (string) Symbol or address of the token
 * Returns:
 *   - d_token_t* : (double) the token price
 */
static inline in3_ret_t rpc_call_zksync_get_token_price(in3_rpc_handle_ctx_t* ctx, d_token_t** res, char* token) {
  char*     jpayload = sprintx("\"%S\"", (char*) token);
  in3_ret_t r        = req_send_sub_request(ctx->req, "zksync_get_token_price", jpayload, NULL, res, NULL);
  _free(jpayload);
  return r;
}
#define FN_ZKSYNC_GET_TOKEN_PRICE "zksync_get_token_price"

/**
 * calculates the fees for a transaction.
 *
 *
 * Parameters:
 *
 *   - char*    txType  : (string) The Type of the transaction "Withdraw" or "Transfer"
 *   - uint8_t* address : (address) the address of the receipient
 *   - char*    token   : (string) the symbol or address of the token to pay
 * Returns:
 *   - d_token_t* : ([object Object]) the fees split up into single values
 */
static inline in3_ret_t rpc_call_zksync_get_tx_fee(in3_rpc_handle_ctx_t* ctx, d_token_t** res, char* txType, uint8_t* address, char* token) {
  char*     jpayload = sprintx("\"%S\",\"%B\",\"%S\"", (char*) txType, bytes(address, 20), (char*) token);
  in3_ret_t r        = req_send_sub_request(ctx->req, "zksync_get_tx_fee", jpayload, NULL, res, NULL);
  _free(jpayload);
  return r;
}
#define FN_ZKSYNC_GET_TX_FEE "zksync_get_tx_fee"

/**
 * returns private key used for signing zksync-transactions
 *
 * Returns:
 *   - bytes_t : (bytes) the raw private key configured based on the signers seed
 */
static inline in3_ret_t rpc_call_zksync_sync_key(in3_rpc_handle_ctx_t* ctx, bytes_t* _res) {
  d_token_t* res = NULL;
  in3_ret_t  r   = req_send_sub_request(ctx->req, "zksync_sync_key", "", NULL, &res, NULL);
  if (!r) *_res = d_bytes(res);
  return r;
}
#define FN_ZKSYNC_SYNC_KEY "zksync_sync_key"

/**
 * sends a deposit-transaction and returns the opId, which can be used to tradck progress.
 *
 *
 * Parameters:
 *
 *   - bytes_t  amount                       : (uint256) the value to deposit in wei (or smallest token unit)
 *   - char*    token                        : (string) the token as symbol or address
 *   - bool     approveDepositAmountForERC20 : (bool) if true and in case of an erc20-token, the client will send a approve transaction first, otherwise it is expected to be already approved.
 *   - uint8_t* account                      : (address) address of the account to send the tx from. if not specified, the first available signer will be used.
 * Returns:
 *   - d_token_t* : ([object Object]) the receipt and the receipopId. You can use `zksync_ethop_info` to follow the state-changes.
 */
static inline in3_ret_t rpc_call_zksync_deposit(in3_rpc_handle_ctx_t* ctx, d_token_t** res, bytes_t amount, char* token, bool approveDepositAmountForERC20, uint8_t* account) {
  char*     jpayload = sprintx("\"%B\",\"%S\",%i,\"%B\"", (bytes_t) amount, (char*) token, (int) approveDepositAmountForERC20, bytes(account, 20));
  in3_ret_t r        = req_send_sub_request(ctx->req, "zksync_deposit", jpayload, NULL, res, NULL);
  _free(jpayload);
  return r;
}
#define FN_ZKSYNC_DEPOSIT "zksync_deposit"

/**
 * sends a zksync-transaction and returns data including the transactionHash.
 *
 *
 * Parameters:
 *
 *   - uint8_t* to      : (address) the receipient of the tokens
 *   - bytes_t  amount  : (uint256) the value to transfer in wei (or smallest token unit)
 *   - char*    token   : (string) the token as symbol or address
 *   - uint8_t* account : (address) address of the account to send the tx from. if not specified, the first available signer will be used.
 * Returns:
 *   - d_token_t* : (zk_receipt) the transactionReceipt. use `zksync_tx_info` to check the progress.
 */
static inline in3_ret_t rpc_call_zksync_transfer(in3_rpc_handle_ctx_t* ctx, d_token_t** res, uint8_t* to, bytes_t amount, char* token, uint8_t* account) {
  char*     jpayload = sprintx("\"%B\",\"%B\",\"%S\",\"%B\"", bytes(to, 20), (bytes_t) amount, (char*) token, bytes(account, 20));
  in3_ret_t r        = req_send_sub_request(ctx->req, "zksync_transfer", jpayload, NULL, res, NULL);
  _free(jpayload);
  return r;
}
#define FN_ZKSYNC_TRANSFER "zksync_transfer"

/**
 * withdraws the amount to the given `ethAddress` for the given token.
 *
 *
 * Parameters:
 *
 *   - uint8_t* ethAddress : (address) the receipient of the tokens in L1
 *   - bytes_t  amount     : (uint256) the value to transfer in wei (or smallest token unit)
 *   - char*    token      : (string) the token as symbol or address
 *   - uint8_t* account    : (address) address of the account to send the tx from. if not specified, the first available signer will be used.
 * Returns:
 *   - d_token_t* : (zk_receipt) the transactionReceipt. use `zksync_tx_info` to check the progress.
 */
static inline in3_ret_t rpc_call_zksync_withdraw(in3_rpc_handle_ctx_t* ctx, d_token_t** res, uint8_t* ethAddress, bytes_t amount, char* token, uint8_t* account) {
  char*     jpayload = sprintx("\"%B\",\"%B\",\"%S\",\"%B\"", bytes(ethAddress, 20), (bytes_t) amount, (char*) token, bytes(account, 20));
  in3_ret_t r        = req_send_sub_request(ctx->req, "zksync_withdraw", jpayload, NULL, res, NULL);
  _free(jpayload);
  return r;
}
#define FN_ZKSYNC_WITHDRAW "zksync_withdraw"

/**
 * withdraws all tokens for the specified token as a onchain-transaction. This is useful in case the zksync-server is offline or tries to be malicious.
 *
 *
 * Parameters:
 *
 *   - char* token : (string) the token as symbol or address
 * Returns:
 *   - d_token_t* : (eth_transactionReceipt) the transactionReceipt
 */
static inline in3_ret_t rpc_call_zksync_emergency_withdraw(in3_rpc_handle_ctx_t* ctx, d_token_t** res, char* token) {
  char*     jpayload = sprintx("\"%S\"", (char*) token);
  in3_ret_t r        = req_send_sub_request(ctx->req, "zksync_emergency_withdraw", jpayload, NULL, res, NULL);
  _free(jpayload);
  return r;
}
#define FN_ZKSYNC_EMERGENCY_WITHDRAW "zksync_emergency_withdraw"

/**
 * calculate the public key based on multiple public keys signing together using schnorr musig signatures.
 *
 *
 * Parameters:
 *
 *   - bytes_t pubkeys : (bytes) concatinated packed publickeys of the signers. the length of the bytes must be `num_keys * 32`
 * Returns:
 *   - bytes_t : (bytes32) the compact public Key
 */
static inline in3_ret_t rpc_call_zksync_aggregate_pubkey(in3_rpc_handle_ctx_t* ctx, bytes_t* _res, bytes_t pubkeys) {
  d_token_t* res      = NULL;
  char*      jpayload = sprintx("\"%B\"", (bytes_t) pubkeys);
  in3_ret_t  r        = req_send_sub_request(ctx->req, "zksync_aggregate_pubkey", jpayload, NULL, &res, NULL);
  _free(jpayload);
  if (!r) *_res = d_bytes(res);
  return r;
}
#define FN_ZKSYNC_AGGREGATE_PUBKEY "zksync_aggregate_pubkey"

#endif