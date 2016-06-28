<?php

/**
 * @file
 * dmonitor module admin functions.
 */

/**
 * FAPI callback.
 * D-Monitor settings form definition.
 */
function dmonitor_settings_form($form, &$form_state) {
  
  $form['dmonitor_secret'] = array(
    '#title' => 'Secret key',
    '#description' => 'You can find your account\'s secret key at http://www.d-monitor.com/account',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('dmonitor_secret', ''),
  );
  
  $form['#submit'][] = 'dmonitor_settings_form_submit';
  return system_settings_form($form);
}


/**
 * FAPI callback / D-Monitor settings form submit callback.
 * 
 * Send the current website's address home so it can be.
 * registered with its account.
 */
function dmonitor_settings_form_submit($form, &$form_state) {
  
  $website = sprintf(
    "%s://%s",
    isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] != 'off' ? 'https' : 'http',
    $_SERVER['SERVER_NAME']
  );
  
  $secret = $form_state['values']['dmonitor_secret'];
  
  $url = 'http://d-monitor.snixz.nl/dmonitor/register-website';
  $data = array(
  'website' => $website,
  'secret' => $secret,
  );
  
  $full_url = url($url, array('query' => $data, 'method' => 'GET'));
  $result = drupal_http_request($full_url);
  
  if (!in_array( $result->code, array(200, 304))) { 
    
    unset($form_state['values']['dmonitor_secret']);
    form_set_error('', 'Unable to register your website address with the D-Monitor server. Please try again. Contact D-Monitor if this error persists. (' . $result->code . ')');
  }
  else {
    
    // Process received data.
    $data = unserialize($result->data);
    
    if($data->error[0]) {
      foreach($data->error as $error) {        
        form_set_error('', $error['message']);
      }
    }
    else {
      foreach($data->notice as $notice) {
        drupal_set_message($notice);
      }
    }
  }
}