.. index::
    single: Core; Bundles; Form Types

Form Types
==========

Checkbox URL Label
------------------

The form type ``cmf_core_checkbox_url_label`` is based on the ``checkbox``
type and adds features useful for the classical "accept terms" check.
The difference to a normal checkbox is that the label is linked to a document,
typically a "terms and conditions" document. When using this type, you
additionally specify ``content_ids``, which are understood by the
:doc:`DynamicRouter <../routing/dynamic>`, along with replacement tokens::

    $form->add('terms', 'cmf_core_checkbox_url_label', array(
        'label' => 'I have seen the <a href="%team%">Team</a> and <a href="%more%">More</a> pages ...',
        'content_ids' => array(
            '%team%' => '/cms/content/static/team',
            '%more%' => '/cms/content/static/more',
        ),
    ));

The form type automatically generates the routes for the specified content and
passes the routes to the ``trans`` Twig helper for replacement in the label.
