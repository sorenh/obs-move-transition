#include "move-transition.h"
#include <obs-module.h>

struct move_filter_info {
	obs_source_t *source;
};

static void *move_filter_create(obs_data_t *settings, obs_source_t *source)
{
	struct move_filter_info *move_filter =
		bzalloc(sizeof(struct move_filter_info));
	move_filter->source = source;
	return move_filter;
}

static void move_filter_destroy(void *data)
{
	struct move_filter_info *move_filter = data;
	bfree(move_filter);
}

void prop_list_add_easings(obs_property_t *p);
void prop_list_add_easing_functions(obs_property_t *p);
void prop_list_add_positions(obs_property_t *p);
void prop_list_add_transitions(obs_property_t *p);
void prop_list_add_scales(obs_property_t *p);

bool prop_list_add_source(obs_scene_t *scene, obs_sceneitem_t *item, void *data)
{
	obs_property_t *p = data;
	const char *name = obs_source_get_name(obs_sceneitem_get_source(item));
	obs_property_list_add_string(p, name, name);
	return true;
}

static obs_properties_t *move_filter_properties(void *data)
{
	struct move_filter_info *move_filter = data;
	obs_properties_t *ppts = obs_properties_create();
	obs_property_t *p;
	obs_source_t *parent = obs_filter_get_parent(move_filter->source);
	obs_scene_t *scene = obs_scene_from_source(parent);

	if (scene) {
		p = obs_properties_add_list(ppts, S_SOURCE,
					    obs_module_text("Source"),
					    OBS_COMBO_TYPE_LIST,
					    OBS_COMBO_FORMAT_STRING);
		obs_scene_enum_items(scene, prop_list_add_source, p);
	}

	//Matched items
	obs_properties_t *group = obs_properties_create();
	p = obs_properties_add_list(group, S_EASING_MATCH,
				    obs_module_text("Easing"),
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, obs_module_text("NoOverride"),
				  NO_OVERRIDE);
	prop_list_add_easings(p);
	p = obs_properties_add_list(group, S_EASING_FUNCTION_MATCH,
				    obs_module_text("EasingFunction"),
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, obs_module_text("NoOverride"),
				  NO_OVERRIDE);
	prop_list_add_easing_functions(p);

	p = obs_properties_add_list(group, S_TRANSITION_MATCH,
				    obs_module_text("Transition"),
				    OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);
	prop_list_add_transitions(p);
	p = obs_properties_add_list(group, S_TRANSITION_SCALE,
				    obs_module_text("TransitionScaleType"),
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, obs_module_text("NoOverride"),
				  NO_OVERRIDE);
	prop_list_add_scales(p);

	obs_properties_t *curve_group = obs_properties_create();

	obs_properties_add_float_slider(curve_group, S_CURVE_MATCH,
					obs_module_text("Curve"), -2.0, 2.0,
					0.01);
	obs_properties_add_group(group, S_CURVE_OVERRIDE_MATCH,
				 obs_module_text("CurveOverride"),
				 OBS_GROUP_CHECKABLE, curve_group);

	obs_properties_add_group(ppts, S_MOVE_MATCH,
				 obs_module_text("MoveMatch"), OBS_GROUP_NORMAL,
				 group);

	//Move in
	group = obs_properties_create();
	p = obs_properties_add_list(group, S_EASING_IN,
				    obs_module_text("Easing"),
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, obs_module_text("NoOverride"),
				  NO_OVERRIDE);
	prop_list_add_easings(p);
	p = obs_properties_add_list(group, S_EASING_FUNCTION_IN,
				    obs_module_text("EasingFunction"),
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, obs_module_text("NoOverride"),
				  NO_OVERRIDE);
	prop_list_add_easing_functions(p);

	p = obs_properties_add_list(group, S_ZOOM_IN, obs_module_text("Zoom"),
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, obs_module_text("NoOverride"),
				  NO_OVERRIDE);
	obs_property_list_add_int(p, obs_module_text("No"), ZOOM_NO);
	obs_property_list_add_int(p, obs_module_text("Yes"), ZOOM_YES);

	p = obs_properties_add_list(group, S_POSITION_IN,
				    obs_module_text("Position"),
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, obs_module_text("NoOverride"),
				  NO_OVERRIDE);
	prop_list_add_positions(p);

	p = obs_properties_add_list(group, S_TRANSITION_IN,
				    obs_module_text("Transition"),
				    OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);
	prop_list_add_transitions(p);

	curve_group = obs_properties_create();

	obs_properties_add_float_slider(curve_group, S_CURVE_IN,
					obs_module_text("Curve"), -2.0, 2.0,
					0.01);
	obs_properties_add_group(group, S_CURVE_OVERRIDE_IN,
				 obs_module_text("CurveOverride"),
				 OBS_GROUP_CHECKABLE, curve_group);

	obs_properties_add_group(ppts, S_MOVE_IN, obs_module_text("MoveIn"),
				 OBS_GROUP_NORMAL, group);

	//Move out
	group = obs_properties_create();
	p = obs_properties_add_list(group, S_EASING_OUT,
				    obs_module_text("Easing"),
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, obs_module_text("NoOverride"),
				  NO_OVERRIDE);
	prop_list_add_easings(p);
	p = obs_properties_add_list(group, S_EASING_FUNCTION_OUT,
				    obs_module_text("EasingFunction"),
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, obs_module_text("NoOverride"),
				  NO_OVERRIDE);
	prop_list_add_easing_functions(p);

	p = obs_properties_add_list(group, S_ZOOM_OUT, obs_module_text("Zoom"),
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, obs_module_text("NoOverride"),
				  NO_OVERRIDE);
	obs_property_list_add_int(p, obs_module_text("No"), ZOOM_NO);
	obs_property_list_add_int(p, obs_module_text("Yes"), ZOOM_YES);

	p = obs_properties_add_list(group, S_POSITION_OUT,
				    obs_module_text("Position"),
				    OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, obs_module_text("NoOverride"),
				  NO_OVERRIDE);
	prop_list_add_positions(p);

	p = obs_properties_add_list(group, S_TRANSITION_OUT,
				    obs_module_text("Transition"),
				    OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);
	prop_list_add_transitions(p);

	curve_group = obs_properties_create();

	obs_properties_add_float_slider(curve_group, S_CURVE_OUT,
					obs_module_text("Curve"), -2.0, 2.0,
					0.01);
	obs_properties_add_group(group, S_CURVE_OVERRIDE_OUT,
				 obs_module_text("CurveOverride"),
				 OBS_GROUP_CHECKABLE, curve_group);

	obs_properties_add_group(ppts, S_MOVE_OUT, obs_module_text("MoveOut"),
				 OBS_GROUP_NORMAL, group);

	return ppts;
}

void move_filter_defaults(obs_data_t *settings)
{
	obs_data_set_default_int(settings, S_EASING_MATCH, NO_OVERRIDE);
	obs_data_set_default_int(settings, S_EASING_IN, NO_OVERRIDE);
	obs_data_set_default_int(settings, S_EASING_OUT, NO_OVERRIDE);
	obs_data_set_default_int(settings, S_EASING_FUNCTION_MATCH,
				 NO_OVERRIDE);
	obs_data_set_default_int(settings, S_EASING_FUNCTION_IN, NO_OVERRIDE);
	obs_data_set_default_int(settings, S_EASING_FUNCTION_OUT, NO_OVERRIDE);
	obs_data_set_default_int(settings, S_POSITION_IN, NO_OVERRIDE);
	obs_data_set_default_int(settings, S_ZOOM_IN, NO_OVERRIDE);
	obs_data_set_default_int(settings, S_POSITION_OUT, NO_OVERRIDE);
	obs_data_set_default_int(settings, S_ZOOM_OUT, NO_OVERRIDE);
	obs_data_set_default_int(settings, S_TRANSITION_SCALE, NO_OVERRIDE);
}

void move_filter_video_render(void *data, gs_effect_t *effect)
{
	UNUSED_PARAMETER(effect);
	struct move_filter_info *filter = data;
	obs_source_skip_video_filter(filter->source);
}

static const char *move_filter_get_name(void *type_data)
{
	UNUSED_PARAMETER(type_data);
	return obs_module_text("MoveTransitionOverrideFilter");
}

struct obs_source_info move_transition_override_filter = {
	.id = "move_transition_override_filter",
	.type = OBS_SOURCE_TYPE_FILTER,
	.output_flags = OBS_SOURCE_VIDEO,
	.get_name = move_filter_get_name,
	.create = move_filter_create,
	.destroy = move_filter_destroy,
	.get_properties = move_filter_properties,
	.get_defaults = move_filter_defaults,
	.video_render = move_filter_video_render};
