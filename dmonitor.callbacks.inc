<?php

/**
 * @file
 * dmonitor callback functions.
 * Part of D-Monitor module.
 */

/**
 * Menu callback.
 * Return a list of active modules.
 * 
 * @todo Search not only sites/all/modules but all possible folders.
 */
function dmonitor_json_list($secret) {
  
  // @todo Validate $secret
  
  // @todo Validate calling server
  
  // Create an array of present modules.
  // For now we only take in account sites/all/modules.
  $result = array();
  foreach($info_files = getDirContents(DRUPAL_ROOT . '/sites/all/modules') as $file) {
    $result[] = parse_ini_file($file);
  }
  
  print drupal_json_output($result);
}


/**
 * Menu callback.
 * Upon request let 'm know we're there.
 */
function dmonitor_check_installed($secret) {
  
  if(dmonitor_permitted($secret)) {    
    header('X-dmonitor: Ok');
  }
}


/**
 * API callback.
 * Check whether request is permitted.
 */
function dmonitor_permitted($secret) {
  
  // D-Monitor IP: 77.72.144.123
  if($_SERVER['REMOTE_ADDR'] != '77.72.144.123') {
    return FALSE;
  }
  
  if($secret != variable_get('dmonitor_secret', '')) {
    return FALSE;
  }
  
  return TRUE;
}


/**
 * Helper.
 * Generate a .info file list based on a start path.
 */
function getDirContents($dir, &$results = array()){
    $files = scandir($dir);

    foreach($files as $key => $value){
        $path = realpath($dir.DIRECTORY_SEPARATOR.$value);
        if(!is_dir($path) ) {
          if(preg_match("/\.info$/", $value)) { 
            $results[] = $path;
          }
        } else if($value != "." && $value != "..") {
            getDirContents($path, $results);
            //$results[] = $path;
        }
    }

    return $results;
}