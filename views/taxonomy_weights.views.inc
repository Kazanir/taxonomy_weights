<?php

/**
 * Implements hook_views_data_alter().
 */
function taxonomy_weights_views_data_alter(&$data) {
  $data['taxonomy_index']['weight'] = array(
    'title' => t('Node index weight'),
    'help' => t('Order nodes separately for each taxonomy term.'),
    'field' => array(
      'handler' => 'taxonomy_weights_handler_field_weight',
      'skip base' => 'taxonomy_term_data',
      'title' => t('Node index weight'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'skip base' => 'taxonomy_term_data',
      'title' => t('Node index weight'),
    ),
  );
}




