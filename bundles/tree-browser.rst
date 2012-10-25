TreeBrowserBundle
=================

The `TreeBrowserBundle <https://github.com/symfony-cmf/TreeBrowserBundle#readme>`_
provides integration with :doc:`/bundles/tree` to provide a tree navigation on top of a PHPCR repository.

This bundle consists of two parts:

 * Generic Tree Browser with a TreeInterface
 * PHPCR tree implementation and GUI for a PHPCR browser

.. index:: TreeBrowserBundle

Dependencies
------------

* `TreeBundle <https://github.com/symfony-cmf/TreeBundle#readme>`_

Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_tree_browser``

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_tree_browser:
            session:  default

Usage
-----

You have select.js and init.js which are a wrapper to build a jquery tree. Use
them with SelectTree.initTree resp. AdminTree.initTree

 * SelectTree in select.js is a tree to select a node to put its id into a field
 * AdminTree in init.js is a tree to create, move and edit nodes

Both have the following options when creating:

 * config.rootNode: id to the root node of your tree, defaults to "/"
 * config.path.expanded: tree path where the tree should be expanded to at the moment
 * config.path.preloaded: tree path what node should be preloaded for faster user experience
 * config.ajax.children_url: Url to the controller that provides the children of a node
 * config.selector: jquery selector where to hook in the js tree

select.js only
~~~~~~~~~~~~~~

 * config.output: where to write the id of the selected node

init.js only
~~~~~~~~~~~~

 * config.ajax.move_url: Url to the controller that handles the move operation
 * config.doctypes: array to manage creating new nodes
 * config.routecollection: array indexed with the className attribute of nodes plus .routes.edit and .routes.delete that maps to routes for the edit and remove operations.

Examples
--------

Look at the templates in the Sonata Admin Bundle for examples how to build the tree:

* `init.js <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/master/Resources/views/Tree/tree.html.twig>`_
* `select.js <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/master/Resources/views/Form/form_admin_fields.html.twig>`_ (look for doctrine_phpcr_type_tree_model_widget)