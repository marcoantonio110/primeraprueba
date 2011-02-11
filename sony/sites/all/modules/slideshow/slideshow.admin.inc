<?php
// $Id: slideshow.admin.inc,v 1.1.2.1 2009/04/05 17:38:34 timcn Exp $

/**
 * @file
 * Contains administrative functions for slideshow.module.
 */

/**
 * Implementation of hook_settings().
 */
function slideshow_admin_settings() {
  $maximum_size = variable_get('upload_max_resolution', 0);
  if (!preg_match('~^[1-9]\d*x[1-9]\d*$~', $maximum_size)) {
    $maximum_size = t('not set');
  }

  $form['slideshow_default_location'] = array(
    '#type' => 'select',
    '#title' => t('Default display location'),
    '#description' => t('Select where the slideshow appears by default. If you select %above, users have to explicitly active the slideshow for each post.', array('%above' => t('Above'))),
    '#options' => array(
      SLIDESHOW_LOCATION_NONE => t('Nowhere'),
      SLIDESHOW_LOCATION_ABOVE => t('Above'),
      SLIDESHOW_LOCATION_BELOW => t('Below'),
      SLIDESHOW_LOCATION_TAG => t('Within')
    ),
    '#default_value' => variable_get('slideshow_default_location', SLIDESHOW_DEFAULT_LOCATION),
  );

  $form['slideshow_default_align'] = array(
    '#type' => 'select',
    '#title' => t('Default alignment'),
    '#description' => t('Select how the slideshow box should be displayed by default.'),
    '#options' => array(
      SLIDESHOW_ALIGN_FULL => t('Full width'),
      SLIDESHOW_ALIGN_LEFT => t('Left'),
      SLIDESHOW_ALIGN_RIGHT => t('Right')
    ),
    '#default_value' => variable_get('slideshow_default_align', SLIDESHOW_DEFAULT_ALIGN),
  );

  $form['slideshow_default_dimensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Default image size'),
    '#default_value' => variable_get('slideshow_default_dimensions', SLIDESHOW_DEFAULT_DIMENSIONS),
    '#size' => 20,
    '#maxlength' => 20,
    '#description' => t('The maximum size of images displayed in a slideshow. Cannot be bigger than the maximum image size for upload module (which is @maximum-size).', array('@maximum-size' => $maximum_size)),
    '#field_suffix' => t('<kbd>WIDTHxHEIGHT</kbd>'),
  );

  $form['slideshow_image_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Slideshow image path'),
    '#default_value' => variable_get('slideshow_image_path', SLIDESHOW_DEFAULT_IMAGE_PATH),
    '#field_prefix' => variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC) == FILE_DOWNLOADS_PUBLIC ? file_create_url('') : '&lt;'. t('Private') .'&gt;/',
    '#description' => t('Subdirectory in the global file directory where resized images for slideshows will be stored.'),
  );

  $form['slideshow_suppress_on_teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Suppress slideshow display on teasers'),
    '#default_value' => variable_get('slideshow_suppress_on_teaser', SLIDESHOW_DEFAULT_SHOW_IN_TEASER),
    '#description' => t('Check this box to suppress slideshow display on teasers.'),
  );

  return system_settings_form($form);
}

function slideshow_admin_settings_validate($form, &$form_state) {
  // Test for a valid image size
  if (!preg_match('~^[1-9]\d*x[1-9]\d*$~', $form_state['values']['slideshow_default_dimensions'])) {
    form_set_error('slideshow_default_dimensions', t('The specified default slideshow image size is invalid.'));
  }
}
