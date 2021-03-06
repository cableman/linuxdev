<?php

/**
 * @file
 * The functions here is used to create an new form element
 * (background_upload_form), which can be used to upload images. It also gives
 * the possibility to flag a image for deletion or active background image.
 *
 */

/**
 * Implements hook_elements().
 *
 */
function dynamic_background_element_info() {
  $types = array();
  $types['background_upload_form'] = array(
    '#input' => TRUE,
    '#theme' => array('background_upload_form'),
    '#theme_wrappers' => array('form_element'),
    '#process' => array('dynamic_background_upload_form_process'),
    '#element_validate' => array('dynamic_background_upload_form_validate'),
  );
  return $types;
}

/**
 * Implements hook_theme(). This defines the default theming function
 * for the background_upload_form element.
 *
 */
function dynamic_background_theme() {
  return array(
    'background_upload_form' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Theming function for background_upload_form.
 *
 */
function theme_background_upload_form($element) {
  $output = '';
  $output .= '<div class="background-upload-form">';
  $output .= drupal_render_children($element['element']);
  $output .= '</div>';
  return $output;
}

/**
 * Implements hook_form_process(). Handles the background_upload_form
 * element processing and building.
 *
 */
function dynamic_background_upload_form_process($element, &$form_state, $form) {
  $element['#tree'] = TRUE;

  // Merge the default values.
  if (!isset($element['#value'])) {
    $element['#value'] = $element['#default_value'];
  }
  else {
    if (is_array($element['#default_value'])) {
      $element['#value'] = array_merge($element['#value'], $element['#default_value']);
    }
  }

  // If image is already uploaded
  if (!empty($element['#value']['picture'])) {
    // Create image preview thumbnail.
    $image = array(
      'style_name' => 'thumbnail',
      'path' => $element['#value']['picture'],
      'alt' => t('Image preview'),
      'title' => t('Image preview'),
    );
    $picture = theme('image_style', $image);
    $element['current_picture'] = array(
      '#markup' => $picture,
    );

    // Checkbox to indicate if this image should be used as background image.
    $element['picture_use'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use picture as background'),
      '#default_value' => isset($element['#value']['picture_use']) ? $element['#value']['picture_use'] : '',
      '#prefix' => '<div class="picture-use">',
      '#suffix' => '</div>',
    );

    // Checkbox to control deletion of the image.
    $element['picture_delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete picture'),
      '#default_value' => isset($element['#value']['picture_delete']) ? $element['#value']['picture_delete'] : '',
    );

    // Image path used by the administraion UI.
    $element['picture'] = array(
      '#type' => 'hidden',
      '#default_value' => isset($element['#value']['picture']) ? $element['#value']['picture'] : '',
    );

    // Image file id, used when the file is deleted.
    $element['fid'] = array(
      '#type' => 'hidden',
      '#default_value' => isset($element['#value']['fid']) ? $element['#value']['fid'] : '',
    );
  }
  else {
    // No image uploaded, so display image upload field.
    $element['picture'] = array(
      '#type' => 'file',
      '#description' => t('Select image file to upload'),
    );
  }

  // Load styling and js.
  drupal_add_css(drupal_get_path('module', 'dynamic_background') . '/css/dynamic_background.admin.css', 'module');
  drupal_add_js(drupal_get_path('module', 'dynamic_background') . '/js/dynamic_background.js', 'file');

  return $element;
}

/**
 * Validation of the background_upload_form element. It have to return the
 * $form.
 *
 */
function dynamic_background_upload_form_validate($element, &$form_state) {
  return $element;
}