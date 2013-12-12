<?php

//Administration posible routes
function latch_pairing($uid) {		
	$title = isset($uid->title) ? $uid->title : $uid->name;
  	drupal_set_title(check_plain($title));

  	$query = db_query("SELECT * FROM {latch} WHERE uid=%d",$uid);
  	$result = db_fetch_object($query);
  	if(!$result) 
		$content = build_pairingform($uid);
	else{
		$content = "<br><div>Click the button to unpair with your Latch account</div>";
		$content .= build_unpairingform($uid);
	}
	return $content;
}


function pairingform($form_state){
	
	$form['token'] = array(
	  '#type' => 'textfield',	 
	  '#title' => t('Latch Pairing Token'),
	  '#default_value' => '',	  
	  '#suffix' => "</ul>",
	  '#size' => 20,
	  '#maxlength' => 10,
	  '#required' => TRUE,
	);

	$form['pair'] = array(
		  '#type' => 'submit',	   
		  '#value' => t('Pair Account'),	  
	);
	
	return $form;
}

function build_pairingform($uid) {  
	return drupal_get_form('pairingform',$uid);
}

function pairingform_validate($form, &$form_state) {
  //TODO: Add validation for pairingtoken
}

function pairingform_submit($form, $form_state) {
	$uid = $form['#parameters']['2'];

	$token = $form_state['values']['token'];
	pairAccount($token,$uid);
}

function unpairingform($form_state){

	$form['unpair'] = array(
		  '#type' => 'submit',	   
		  '#value' => t('Unpair Account'),	  
	);
	
	return $form;
}

function build_unpairingform($uid) {  
	return drupal_get_form('unpairingform',$uid);
}

function unpairingform_submit($form, $form_state) {

	$uid = $form['#parameters']['2'];
	unpairAccount($uid);
}


function pairAccount($token, $uid) {			
	$appSecret = variable_get('latch_secret', '');
	$appId = variable_get('latch_appid', '');

	if (!empty($appId) && !empty($appSecret)) {
		$api = new Latch($appId, $appSecret);
		
		$pairResponse = $api->pair($token);
		$responseData = $pairResponse->getData();
		

		if (!empty($responseData)) {
			$accountId = $responseData->{"accountId"};
		}
		
		if (!empty($accountId)) {
			//OK PAIRING
			db_query("INSERT INTO {latch} (uid,latch_account) VALUES (%d,'%s')",$uid,$accountId);
			drupal_set_message('Pairing succes');
		} else {
			//NOK PAIRING
			drupal_set_message('Pairing token not valid','error');
		}
	}	
}

function unpairAccount($uid) {			
	$appSecret = variable_get('latch_secret', '');
	$appId = variable_get('latch_appid', '');
	
	
	if (!empty($appId) && !empty($appSecret)) {
		$api = new Latch($appId, $appSecret);
		
		$query = db_query("SELECT * FROM {latch} WHERE uid=%d" , $uid);
  		$result = db_fetch_object($query);

		$unpairResponse = $api->unpair($result->latch_account);
		$responseData = $unpairResponse->getData();
						
		if (empty($responseError)) {
			db_query("DELETE FROM {latch} WHERE uid=%d",$uid);
		} else {
			//TODO: Handle error while unpairing...
		}
	}	
}