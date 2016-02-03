<?php

class taxonomy_weights_handler_field_weight extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['tid'] = 'tid';
  }

  function render($values) {
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  function views_form(&$form, &$form_state) {
    $field_id = $this->options['id'];
    $form[$field_id] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );

    foreach($this->view->result as $row_index => $row) {
      // We use a textfield instead of a weight element in order to accomodate wide-ranging weights
      // due to a potentially high number of nodes on a single term
      $form[$field_id][$row_index] = array(
        '#type' => 'textfield',
        '#title_display' => 'invisible',
        '#size' => 5,
        '#maxlength' => 6,
        '#element_validate' => 'element_validate_integer',
        '#title' => '',
        '#default_value' => $row->{$this->field_alias},
        '#attributes' => array(
          'class' => array('taxonomy-weight-field'),
        ),
      );
    }
  }

  function views_form_submit($form, &$form_state) {
    $field_id = $this->options['id'];

    $updates = array();
    foreach ($this->view->result as $idx => $row) {
      $value = $form_state['values'][$field_id][$idx];

      if ($row->{$this->field_alias} != $value) {
        $updates[] = (object) array(
          'tid' => $row->{$this->aliases['tid']},
          'nid' => $row->{$this->aliases['nid']},
          'weight' => $value,
        );
      }
    }

    // Do the actual database updates here.
    if (!empty($updates)) {
      $txn = db_transaction();
      foreach ($updates as $u) {
        db_query("UPDATE {taxonomy_index} SET weight = :w WHERE tid = :t AND nid = :n;", array(':w' => $u->weight, ':t' => $u->tid, ':n' => $u->nid));
      }
      unset($txn);
    }

  }



}
