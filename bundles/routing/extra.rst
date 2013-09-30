.. index::
    single: Routing; Bundles

Extra features of the RoutingBundle
===================================

Form Type
---------

The bundle defines a form type that can be used for classical "accept terms"
checkboxes where you place urls in the label. Simply specify
``cmf_routing_terms_form_type`` as the form type name and specify a
label and an array with ``content_ids`` in the options::

    $form->add('terms', 'cmf_routing_terms_form_type', array(
        'label' => 'I have seen the <a href="%team%">Team</a> and <a href="%more%">More</a> pages ...',
        'content_ids' => array(
            '%team%' => '/cms/content/static/team',
            '%more%' => '/cms/content/static/more',
        ),
    ));

The form type automatically generates the routes for the specified content and
passes the routes to the trans twig helper for replacement in the label.
