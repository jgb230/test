/* 
 *jgb
 *
 */

#include <switch.h>

SWITCH_MODULE_LOAD_FUNCTION(mod_jgb_load);
SWITCH_MODULE_DEFINITION(mod_jgb, mod_jgb_load, NULL, NULL);

static switch_status_t switch_jgb_init(switch_codec_t *codec, switch_codec_flag_t flags, const switch_codec_settings_t *codec_settings)
{
		return SWITCH_STATUS_SUCCESS;
}

static switch_status_t switch_jgb_encode(switch_codec_t *codec,
										  switch_codec_t *other_codec,
										  void *decoded_data,
										  uint32_t decoded_data_len,
										  uint32_t decoded_rate, void *encoded_data, uint32_t *encoded_data_len, uint32_t *encoded_rate,
										  unsigned int *flag)
{
	return SWITCH_STATUS_FALSE;
}

static switch_status_t switch_jgb_decode(switch_codec_t *codec,
										  switch_codec_t *other_codec,
										  void *encoded_data,
										  uint32_t encoded_data_len,
										  uint32_t encoded_rate, void *decoded_data, uint32_t *decoded_data_len, uint32_t *decoded_rate,
										  unsigned int *flag)
{
	return SWITCH_STATUS_FALSE;
}

static switch_status_t switch_jgb_destroy(switch_codec_t *codec)
{
	return SWITCH_STATUS_SUCCESS;
}

SWITCH_MODULE_LOAD_FUNCTION(mod_jgb_load)
{
	switch_codec_interface_t *codec_interface;
	/* connect my internal structure to the blank pointer passed to me */
	*module_interface = switch_loadable_module_create_module_interface(pool, modname);
	SWITCH_ADD_CODEC(codec_interface, "jgb Video (passthru)");
	switch_core_codec_add_implementation(pool, codec_interface,
										 SWITCH_CODEC_TYPE_VIDEO, 99, "jgb", NULL, 90000, 90000, 0,
										 0, 0, 0, 0, 1, 1, switch_jgb_init, switch_jgb_encode, switch_jgb_decode, switch_jgb_destroy);
	/* indicate that the module should continue to be loaded */
	return SWITCH_STATUS_SUCCESS;
}
