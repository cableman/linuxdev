<?php

/**
 * @file
 * Administration page callbacks for the annotate module.
 */

/**
 * Form builder. Configure annotations.
 */
function annotate_admin_settings() {
  // Load node types and create options array.
  $types = node_type_get_types();
  foreach ($types as $node_type) {
    $options[$node_type->type] = $node_type->name;
  }


  $form['annotate_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Users may annotate these content types.'),
    '#options' => $options,
    '#default_value' => variable_get('annotate_node_types', array()),
    '#description' => t('A text filed will be available on these content types to make user-specific notes.'),
  );

  $form['#submit'][] = 'annotate_admin_settings_submit';
  return system_settings_form($form);
}

function annotate_admin_settings_submit($form, $form_state) {
  // Loop through each content type checkbox
  foreach ($form_state['values']['annotate_node_types'] as $key => $value) {
    $instance = field_info_instance('node', 'annotation', $key);
    if (!$value) {
      if (!empty($instance)) {
        field_delete_instance($instance);
        watchdog('Annotation', 'Deleted annotation filed from content type: %key', array('%key' => $key));
      }
    }
    else {
      if (empty($instance)) {
        $instance = array(
          'field_name' => 'annotation',
          'entity_type' => 'node',
          'bundle' => $key,
          'label' => t('Annotation'),
          'widget_type' => 'text_textarea_with_summary',
          'settings' => array('display_summary' => TRUE),
          'display' => array(
            'default' => array(
              'type' =>  'text_default',
            ),
            'teaser' => array(
              'type' => 'text_summary_or_trimmed',
            ),
          ),
        );
        $instance = field_create_instance($instance);
        watchdog('Annotation', 'Added annotation field to content type: %key', array('%key' => $key));
      }
    }
  } // End foreach
}
