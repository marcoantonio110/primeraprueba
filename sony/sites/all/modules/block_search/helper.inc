<?php
// $Id: helper.inc,v 1.1 2010/09/20 18:20:59 prasadshir Exp $
/**
 * @file
 * Helper file for block_search.module
 */
 
/**
 * Save block content to the {boxes} and {box_details} tables
 * input parameter $boxdata as an array
 * output parameter $id as number
*/
function block_search_save_box($boxdata){
	$query = "INSERT INTO {boxes} (body, info, format) VALUES ('".addslashes($boxdata['body'])."', '". addslashes($boxdata['info'])."', ".addslashes($boxdata['format']).")";
	//print $query."<br>";
	db_query($query);
	$id = db_last_insert_id('boxes', 'bid');
$query = "INSERT INTO {box_details} (details_bid, title_format, title_style,content_style,blocktitle,blockdetails,ublockid) VALUES ($id,'".addslashes($boxdata['title_format'])."', '".addslashes($boxdata['title_style'])."', '".addslashes($boxdata['content_style'])."','".addslashes($boxdata['plaintitle'])."','".addslashes($boxdata['blockdetails'])."','".addslashes($boxdata['ublockid'])."')";	
//print $query."<br>";
	db_query($query);
	$idetails = db_last_insert_id('box_details', 'details_bid');
	return $id;
}

/**
 * update block content to the {boxes} and {box_details} tables
 * input parameter $boxdata as an array
 * output parameter boxid as number 
*/
function block_search_update_box($boxdata){
	$query = "update boxes set body='".addslashes($boxdata['body'])."', info='".addslashes($boxdata['info'])."', format='".addslashes($boxdata['format'])."' where bid= ".addslashes($boxdata['boxid']);
	db_query($query);
	$query = "update box_details set title_format='".addslashes($boxdata['title_format'])."', title_style='".addslashes($boxdata['title_style'])."',content_style='".addslashes($boxdata['content_style'])."', blocktitle='".addslashes($boxdata['blocktitle'])."',blockdetails='".addslashes($boxdata['blockdetails'])."',ublockid='".addslashes($boxdata['ublockid'])."' where details_bid=".addslashes($boxdata['boxid']);
	db_query($query);
	return $boxdata['boxid'];
}

/**
 * Save block content to the {blocks} table
 * input parameter $blockdata as an array
 * output parameter id as number  
*/
function block_search_save_block($blockdata){
	$query = "INSERT INTO {blocks} (visibility, pages, module, theme, status, weight, delta, cache, region,title) VALUES(".addslashes($blockdata['visibility']).", '".addslashes($blockdata['pages'])."', '".addslashes($blockdata['module'])."', '".addslashes($blockdata['theme'])."', ".addslashes($blockdata['status']).", ".addslashes($blockdata['weight']).", '".addslashes($blockdata['delta'])."', ".addslashes($blockdata['cache']).", '".addslashes($blockdata['region'])."','".addslashes($blockdata['block_title'])."')";
	db_query($query);
	$id = db_last_insert_id('block_search_blocks', 'bid');
	return $id;
}

/**
 * update block content to the {blocks} table
 * input parameter $blockdata as an array
 * output parameter blkid as number   
*/
function block_search_update_block($blockdata){
	init_theme();
	global $theme_key;
	$query="update blocks set visibility='".addslashes($blockdata['visibility'])."', pages='".addslashes($blockdata['pages'])."', module='".addslashes($blockdata['module'])."', theme='".addslashes($blockdata['theme'])."', status='".addslashes($blockdata['status'])."', weight='".addslashes($blockdata['weight'])."', cache='".addslashes($blockdata['cache'])."', region='".addslashes($blockdata['region'])."',  title='".addslashes($blockdata['title'])."' where bid=".addslashes($blockdata['blkid']);
	db_query($query);
	return $blockdata['blkid'];
}

/**
 * Publish block updating {blocks} table
 * input parameter $blocksp as an array
*/
function block_search_publish_block($blocksp){
	foreach ($blocksp as &$value) {
		db_query("update blocks set region = replace(region,'_unpublish','') where bid=". $value);
	}
}

/**
 * Unpublish block updating {blocks} table
 * input parameter $blocksp as an array
*/
function block_search_unpublish_block($blocksp){
	foreach ($blocksp as &$value) {
		db_query("update blocks set region =  CONCAT(region, '_unpublish') where bid=". $value);
	}
}

/**
 * Genrate the content HTML for a block based on the form parameters.
 * input parameter $form_data as an array containning the form field values
 * input parameter $file as the uploaded image data
 * input parameter $for as the filed name for whicht he data is htmalized 
 * output parameter field data as htmalized string 
 */
function block_search_gethtml($form_data, $file, $for) {
	$html = '';
	// Compose block title
	if ($form_data['block_title_format']) {
		$title_tags[] = $form_data['block_title_format'];
	}
	if ($form_data['block_title_style']) {
		foreach ($form_data['block_title_style'] as $style) {
			if ($style) {
				$title_tags[] = $style;
			}
		}
	}
	if ($title_tags) {
		$title = '<' . implode('><', $title_tags) . '>';
		$title .= $form_data['block_title'];
		$title .= '</' . implode('></', array_reverse($title_tags)) . '>'; // We need to reverse to correctly close tags
	}else{
		$title = $form_data['block_title'];
	}// Done composing block title
	
	// Compose block content
	if ($form_data['block_content_style']) {
		foreach ($form_data['block_content_style'] as $style) {
			if ($style) {
				$content_tags[] = $style;
			}
		}
	}
	if ($content_tags) {
		$body = '<' . implode('><', $content_tags) . '>';
		$body .= $form_data['block_content'];
		$body .= '</' . implode('></', array_reverse($content_tags)) . '>'; // We need to reverse to correctly close tags
	}else{
		$body = $form_data['block_content'];
	}
	if($file->destination){
		$img	= '<img src="/'.$file->destination.'" hspace="2" vspace="2" align="left" />';
	}// Done composing block Content
	
	if($for == 'body'){
		$html = $title . $img . $body;	
	}elseif($for == 'title'){
		$html = $title;
	}
  return $html;
} // function block_search_gethtml