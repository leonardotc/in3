/** @file 
 * json-parser.
 * 
 * The parser can read from :
 * - json
 * - bin
 * 
 * When reading from json all '0x'... values will be stored as bytes_t. If the value is lower than 0xFFFFFFF, it is converted as integer.
 * 
 * 
 * */

#include "bytes.h"
#include "stringbuilder.h"
#include <stdint.h>

#ifndef __DATA_H__
#define __DATA_H__

typedef uint16_t d_key_t;
/** type of a token. */
typedef enum {
  T_BYTES   = 0, /**< content is stored as data ptr. */
  T_STRING  = 1, /**<content is stored a c-str*/
  T_ARRAY   = 2, /**< the node is an array with the length stored in length */
  T_OBJECT  = 3, /**< the node is an object with properties*/
  T_BOOLEAN = 4, /**< boolean with the value stored in len */
  T_INTEGER = 5, /**< a integer with the value stored */
  T_NULL    = 6  /**< a NULL-value */
} d_type_t;

/** a token holding any kind of value. 
 * 
 * use d_type,  d_len or the cast-function to get the value.
 */
typedef struct item {
  uint32_t len;  /**< the length of the content (or number of properties) depending +  type. */
  uint8_t* data; /**< the byte or string-data  */
  d_key_t  key;  /**< the key of the property. */
} d_token_t;

/** internal type used to represent the a range within a string. */
typedef struct str_range {
  char*  data; /**< pointer to the start of the string */
  size_t len;  /**< len of the characters */
} str_range_t;

/** parser for json or binary-data. it needs to freed after usage.*/
typedef struct json_parser {
  d_token_t* items;     /**< the list of all tokens. the first token is the main-token as returned by the parser.*/
  size_t     allocated; /** amount of tokens allocated items */
  size_t     len;       /** number of tokens in items */
  char*      c;         /** pointer to the src-data*/
} json_parsed_t;

/**< 
 * returns the byte-representation of token. 
 * 
 * In case of a number it is returned as bigendian. 
 * booleans as 0x01 or 0x00 
 * and NULL as 0x.
 * Objects or arrays will return 0x.
 */
bytes_t                d_to_bytes(d_token_t* item);
int                    d_bytes_to(d_token_t* item, uint8_t* dst, const int max);                                        /**< writes the byte-representation to the dst. details see d_to_bytes.*/
bytes_t*               d_bytes(const d_token_t* item);                                                                  /**< returns the value as bytes (Carefully, make sure that the token is a bytes-type!)*/
char*                  d_string(const d_token_t* item);                                                                 /**< converts the value as string. Make sure the type is string! */
uint32_t               d_int(const d_token_t* item);                                                                    /**< returns the value as integer. only if type is integer */
uint32_t               d_intd(const d_token_t* item, const uint32_t def_val);                                           /**< returns the value as integer or if NULL the default. only if type is integer */
uint64_t               d_long(const d_token_t* item);                                                                   /**< returns the value as long. only if type is integer or bytes, but short enough */
uint64_t               d_longd(const d_token_t* item, const uint64_t def_val);                                          /**< returns the value as long or if NULL the default. only if type is integer or bytes, but short enough */
bytes_t**              d_create_bytes_vec(const d_token_t* arr);                                                        /** creates a array of bytes from JOSN-array */
static inline d_type_t d_type(const d_token_t* item) { return item == NULL ? T_NULL : (item->len & 0xF0000000) >> 28; } /**< type of the token */
static inline int      d_len(const d_token_t* item) { return item == NULL ? 0 : item->len & 0xFFFFFFF; }                /**< number of elements in the token (only for object or array, other will return 0) */
bool                   d_eq(const d_token_t* a, const d_token_t* b);                                                    /**< compares 2 token and if the value is equal */
d_key_t                key(const char* c);                                                                              /**< generates the keyhash for the given string */
d_key_t                keyn(const char* c, const int len);                                                              /**< generates the keyhash for the given stringrange as defined by len */

d_token_t* d_get(d_token_t* item, const uint16_t key);                          /**< returns the token with the given propertyname (only if item is a object) */
d_token_t* d_get_or(d_token_t* item, const uint16_t key1, const uint16_t key2); /**< returns the token with the given propertyname or if not found, tries the other. (only if item is a object) */
d_token_t* d_get_at(d_token_t* item, const uint32_t index);                     /**< returns the token of an array with the given index */
d_token_t* d_next(d_token_t* item);                                             /**< returns the next sibling of an array or object */

void           d_serialize_binary(bytes_builder_t* bb, d_token_t* t); /**< write the token as binary data into the builder */
json_parsed_t* parse_binary(bytes_t* data);                           /**< parses the data and returns the context with the token, which needs to be freed after usage! */
json_parsed_t* parse_binary_str(char* data, int len);                 /**< parses the data and returns the context with the token, which needs to be freed after usage! */
json_parsed_t* parse_json(char* js);                                  /**< parses json-data, which needs to be freed after usage! */
void           free_json(json_parsed_t* parser_ctx);                  /**< frees the parse-context after usage */
str_range_t    d_to_json(d_token_t* item);                            /**< returns the string for a object or array. This only works for json as string. For binary it will not work! */
char*          d_create_json(d_token_t* item);                        /**< creates a json-string. It does not work for objects if the parsed data were binary!*/

int   json_get_int_value(char* js, char* prop);            /**< parses the json and return the value as int. */
void  json_get_str_value(char* js, char* prop, char* dst); /**< parses the json and return the value as string. */
char* json_get_json_value(char* js, char* prop);           /**< parses the json and return the value as json-string. */

// Helper function to map string to 2byte keys (only for tests or debugging)
char* d_get_keystr(d_key_t k);     /**< returns the string for a key. This only works track_keynames was activated before! */
void  d_track_keynames(uint8_t v); /**< activates the keyname-cache, which stores the string for the keys when parsing. */
void  d_clear_keynames();          /**< delete the cached keynames */

static inline char*    d_get_stringk(d_token_t* r, d_key_t k) { return d_string(d_get(r, k)); }              /**< reads token of a property as string. */
static inline char*    d_get_string(d_token_t* r, char* k) { return d_get_stringk(r, key(k)); }              /**< reads token of a property as string. */
static inline char*    d_get_string_at(d_token_t* r, uint32_t pos) { return d_string(d_get_at(r, pos)); }    /**< reads string at given pos of an array. */
static inline uint32_t d_get_intk(d_token_t* r, d_key_t k) { return d_int(d_get(r, k)); }                    /**< reads token of a property as int. */
static inline uint32_t d_get_intkd(d_token_t* r, d_key_t k, uint32_t d) { return d_intd(d_get(r, k), d); }   /**< reads token of a property as int. */
static inline uint32_t d_get_int(d_token_t* r, char* k) { return d_get_intk(r, key(k)); }                    /**< reads token of a property as int. */
static inline uint32_t d_get_int_at(d_token_t* r, uint32_t pos) { return d_int(d_get_at(r, pos)); }          /**< reads a int at given pos of an array. */
static inline uint64_t d_get_longk(d_token_t* r, d_key_t k) { return d_long(d_get(r, k)); }                  /**< reads token of a property as long. */
static inline uint64_t d_get_longkd(d_token_t* r, d_key_t k, uint64_t d) { return d_longd(d_get(r, k), d); } /**< reads token of a property as long. */
static inline uint64_t d_get_long(d_token_t* r, char* k) { return d_get_longk(r, key(k)); }                  /**< reads token of a property as long. */
static inline uint64_t d_get_long_at(d_token_t* r, uint32_t pos) { return d_long(d_get_at(r, pos)); }        /**< reads long at given pos of an array. */
static inline bytes_t* d_get_bytesk(d_token_t* r, d_key_t k) { return d_bytes(d_get(r, k)); }                /**< reads token of a property as bytes. */
static inline bytes_t* d_get_bytes(d_token_t* r, char* k) { return d_get_bytesk(r, key(k)); }                /**< reads token of a property as bytes. */
static inline bytes_t* d_get_bytes_at(d_token_t* r, uint32_t pos) { return d_bytes(d_get_at(r, pos)); }      /**< reads bytes at given pos of an array. */
static inline bool     d_is_binary_ctx(json_parsed_t* ctx) { return ctx->allocated == 0; }                   /**< check if the parser context was created from binary data. */

typedef struct {
  int        i;
  d_token_t* token;
} d_iterator_t;

static inline d_iterator_t d_iter(d_token_t* parent) { return (d_iterator_t){.i = d_len(parent), .token = parent + 1}; } /**< creates a iterator for a object or array */
static inline bool         d_iter_next(d_iterator_t* const iter) {
  iter->token = d_next(iter->token);
  return iter->i--;
} /**< fetched the next token or null if the end is reached.*/

#endif