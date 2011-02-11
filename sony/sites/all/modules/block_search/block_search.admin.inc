<?php
// $Id: block_search.admin.inc,v 1.1 2010/09/20 18:20:59 prasadshir Exp $ 

/**
 * @file
 * Admin callbacks for block_search.module
 */

/**
 * Display a form to add a block
 * input parameter $form_state as an array
 * output parameter $form as html form
*/
function block_search_admin($form_state = array()) {
	init_theme();
	global $theme_key, $user, $pager_page_array, $pager_total;
	$output = '';
	$noofpages = 10;
	$form = array();
	//-----------------------------------------------
	$result = db_query("SELECT * FROM blocks WHERE theme='".$theme_key."'");
	$old_blocks = array();
	while ($old_block = db_fetch_array($result)) {
		$old_blocks[$old_block['module']][$old_block['delta']] = $old_block;
	}
	$blocks = array();
	$regions = system_region_list($theme_key);
	foreach (module_list() as $module) {
		$module_blocks = module_invoke($module, 'block', 'list');
		if ($module_blocks) {
			foreach ($module_blocks as $delta => $block) {
				if (empty($old_blocks[$module][$delta])) {
					$block['module'] = $module;
					$block['delta']  = $delta;
					$block['theme']  = $theme_key;
					if (!isset($block['pages'])) {
						$block['pages']  = '';
					}
					$block['region'] = $block['status'] ? $block['region'] : BLOCK_REGION_NONE;
					$blocks[] = $block;
				}else {
					$old_blocks[$module][$delta]['info'] = $block['info'];
					if (!empty($old_blocks[$module][$delta]['region']) && !isset($regions[$old_blocks[$module][$delta]['region']])) {
						drupal_set_message(t('The block %info was assigned to the invalid region %region and has been disabled.', array('%info' => $old_blocks[$module][$delta]['info'], '%region' => $old_blocks[$module][$delta]['region'])), 'warning');
						$old_blocks[$module][$delta]['status'] = 0;
						$old_blocks[$module][$delta]['region'] = BLOCK_REGION_NONE;
					}else {
						$old_blocks[$module][$delta]['region'] = $old_blocks[$module][$delta]['status'] ? $old_blocks[$module][$delta]['region'] : BLOCK_REGION_NONE;
					}
					$blocks[] = $old_blocks[$module][$delta];
					unset($old_blocks[$module][$delta]);
				}
			}
		}
	}
	$path_pieces = explode('/', drupal_get_path_alias($_GET['q']));
	if(count($path_pieces)>0){
		if($path_pieces[1]<>''){
			if($path_pieces[1] != 'setregion'){
				for($i=0;$i<count($blocks);$i++){
					$pos = strrpos(strtolower($blocks[$i][info]), $path_pieces[1]);
					if($pos !== false){
						$blockwithsearch[] = $blocks[$i];
					}
				}
				$blocks = $blockwithsearch;
			}
		}
	}
	//------------------------------------------------
	$noofp = count($blocks)/$noofpages;
	$header = array(
		array('data' => t('Block'), 'field' => 'info', 'sort' => 'asc'),
		array('data' => t('module'), 'field' => 'module'),
		array('data' => t('Edit')),
		array('data' => t('Delete')),
		array('data' => t('Region'))
	);
	//----------------------------------------------------
	/* search form starts here */
	$form['searchblock'] = array(
		'#type' => 'textfield',
		'#title' => t('Search Block'),
		'#description' => t('This is the search block field'),
	);
	$form['search'] = array(
		'#type' => 'submit',
		'#value' => t('Search'),
		'#default_value' => t('Search'),
		'#name' => 'search'
	);
	$form['clear'] = array(
		'#type' => 'submit',
		'#value' => t('Clear'),
		'#default_value' => t('Clear'),
		'#name' => 'clear'
	);
	/* search form ends here */  
	//----------------------------------------------------
  	$rows = array();
	
	foreach ($blocks as $item)
	{
		if($item[bid]<>''){
			$strDeleteUrl="";			
			$current_url = "http://" .$_SERVER['HTTP_HOST'] .$_SERVER['REQUEST_URI'];			
			if(strpos($current_url,"block_search/") <= 0){
			$strEditUrl ='<a href=admin/build/block/configure/'.$item[module] .'/'. $item[delta].'?destination=block_search>Edit</a>';			
			$setregionurl='<a href="block_search/setregion/'.base64_encode($item[info]).'/'.base64_encode($item[module]).'/'.base64_encode($item[delta]).'">Set Region</a>';
			if($item[module]=='block'){
				$strDeleteUrl = '<a href=admin/build/block/delete/'. $item[delta].'?destination=block_search>Delete</a>';
			}			
			}else{
			$strEditUrl ='<a href=../admin/build/block/configure/'.$item[module] .'/'. $item[delta].'?destination=block_search>Edit</a>';			
			$setregionurl='<a href="../block_search/setregion/'.base64_encode($item[info]).'/'.base64_encode($item[module]).'/'.base64_encode($item[delta]).'">Set Region</a>';			
			if($item[module]=='block'){
				$strDeleteUrl = '<a href=../admin/build/block/delete/'. $item[delta].'?destination=block_search>Delete</a>';
			}			
			}
			$rows[] = array(
				$item[info],
				$item[module],
				$strEditUrl,
				$strDeleteUrl,
				$setregionurl,
			);	
		}
	}
	$page = isset($_GET['page']) ? $_GET['page'] : '';
	$pager_page_array = explode(',', $page);
	$pager_page_array[0] = max(0, min((int)$pager_page_array[0], ( (int)$pager_total[0]) - 1));
	
	// Add the pager to the output.
	$output = theme('table', $header, $rows);
	$form['page']=array(
		'#value'=>$output,
	);
	return $form;
}


function block_search_admin_submit($form, &$form_state){
	if($form_state['values']['search']=='Search'){
		// if search button is clicked
		drupal_goto($path = 'block_search/'.$form_state['values']['searchblock'], $query = NULL, $fragment = NULL, $http_response_code = 302);
	}elseif($form_state['values']['clear']=='Clear'){
		// if clear button is clicked
		drupal_goto($path = 'block_search');
	}else{
	}
}

function block_search_compare($a, $b) {
	global $theme_key;
	static $regions;
	// We need the region list to correctly order by region.
	if (!isset($regions)) {
    	$regions = array_flip(array_keys(system_region_list($theme_key)));
    	$regions[BLOCK_REGION_NONE] = count($regions);
	}
	// Separate enabled from disabled.
	$status = $b['status'] - $a['status'];
	if ($status) {
		return $status;
	}
	// Sort by region (in the order defined by theme .info file).
	if ((!empty($a['region']) && !empty($b['region'])) && ($place = ($regions[$a['region']] - $regions[$b['region']]))) {
		return $place;
	}
	// Sort by weight.
	$weight = $a['weight'] - $b['weight'];
	if ($weight) {
		return $weight;
	}
	// Sort by title.
	return strcmp($a['info'], $b['info']);
}


function block_search_setregion($form_state = array()){
	init_theme();
	global $theme_key;
		$path_pieces = explode('/', drupal_get_path_alias($_GET['q']));
		$form = array();
$form['blankfield'] = array(
  '#type' => 'item',
'#prefix' => '<table><tr><td colspan=2>Set Region for: '.base64_decode($path_pieces[2]).'</td></tr><tr><td>Theme</td><td>Region</td></tr><tr>', 
);	

  foreach (list_themes() as $key => $theme) {
  if($theme->status == 1){
  $result = db_fetch_object(db_query("select * from blocks where module='".base64_decode($path_pieces[3])."' and delta='".base64_decode($path_pieces[4])."' and theme='".$theme->name."'"));
if($result->region == ''){
$defregion="none";
}else{
$defregion=$result->region;
}
	$regions = system_region_list($theme->name);
  $regions['none']='None';

$form[$theme->name]['themename'] = array(
  '#type' => 'item',
  '#value' => $theme->name,
'#prefix' => '<td>',
'#suffix' => '</td>'  
);
	$form[$theme->name][$theme->name] = array(
		'#type' => 'select',
		'#description' => t('Select which position the block should appear in'),
		'#options' => $regions,
		'#default_value' => $defregion,
		'#prefix' => '<td>',
'#suffix' => '</td></tr><tr>' 
	);
  }
  }
  	$form['module'] = array(
		'#type' => 'hidden',
		'#value' => base64_decode($path_pieces[3]));

  	$form['delta'] = array(
		'#type' => 'hidden',
		'#value' => base64_decode($path_pieces[4]));
		
  	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
		'#prefix' => '<td colspan=2>',
'#suffix' => '</td></tr></table>' 
	);


	return $form;
}


function block_search_setregion_submit($form, &$form_state){
	init_theme();
	global $theme_key;
	foreach (list_themes() as $key => $theme) {
		if($theme->status == 1){
			$num_post = db_result(db_query("select * from blocks where module='".$form_state['values']['module']."' and delta='".$form_state['values']['delta']."' and theme='".$theme->name."'"));
			if($form_state['values'][$theme->name]=='none'){
				$selectedregion ='';
			}else{
				$selectedregion = $form_state['values'][$theme->name];
			}			
			if($num_post > 0){
				db_query("update blocks set status=1, region='".$selectedregion."' where module='".$form_state['values']['module']."' and delta='".$form_state['values']['delta']."' and theme='".$theme->name."'");
			}else{
				db_query("INSERT INTO {blocks} (visibility, pages, custom, title, module, theme, status, weight, delta, cache, region) VALUES(0, '', 0, '', '".$form_state['values']['module']."', '".$theme->name."', 1, 0, '".$delta."', '".BLOCK_NO_CACHE."', '".$selectedregion."')");
			}
		}
	}
	
		drupal_goto($path = 'block_search');	
}