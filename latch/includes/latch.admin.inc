<?php

//Administration posible routes
function latch_admin($arguments) {		
	switch ($arguments) {
		case 'settings':						
			$content = latch_adminsettings();
			break;
	}
	
	return $content;
}


function latch_adminsettings() {			
	$content = "<div>This is the page to set Latch configuration. To get register as a Latch application pleases visit: <a target='blank' href='http://latch.elevenpaths.com'>http://latch.elevenpaths.com</a></div><br>";
	$content .= build_settingsform();
	return $content;
}

//Form with the elements for the module config.
function settingsform($form_state){
	//Getting stored values for settings.
	$appid = variable_get('latch_appid', ' ');
	$secret = variable_get('latch_secret', ' ');
	
	
	$form['appid'] = array(
	  '#type' => 'textfield',	 
	  '#title' => t('Application ID'),
	  '#default_value' => $appid,	  
	  '#suffix' => "</ul>",
	  '#size' => 30,
	  '#maxlength' => 20,
	  '#required' => FALSE,
	);

	$form['secret'] = array(
	  '#type' => 'textfield',	 
	  '#title' => t('Application Secret'),
	  '#default_value' => $secret,	  
	  '#suffix' => "</ul>",
	  '#size' => 55,
	  '#maxlength' => 40,
	  '#required' => FALSE,
	);


	$form['save'] = array(
		  '#type' => 'submit',	   
		  '#value' => t('Save'),	  
	);
	
	return $form;
}

function build_settingsform() {  
	return drupal_get_form('settingsform');
}

function settingsform_validate($form, &$form_state) {
  //TODO: Add validation for appId / Secret (Fixed length?)
}

function settingsform_submit($form, $form_state) {
	$appid = $form_state['values']['appid'];
	variable_set('latch_appid', $appid);	
	$secret = $form_state['values']['secret'];
	variable_set('latch_secret', $secret);

	drupal_goto("admin/settings/latch");
}



