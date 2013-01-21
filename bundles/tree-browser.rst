TreeBrowserBundle
=================

The `TreeBrowserBundle <https://github.com/symfony-cmf/TreeBrowserBundle#readme>`_
provides a tree navigation on top of a PHPCR repository.

This bundle consists of two parts:

 * Generic Tree Browser with a TreeInterface
 * PHPCR tree implementation and GUI for a PHPCR browser

.. index:: TreeBrowserBundle

Dependencies
------------

* `FOSJsRoutingBundle <https://github.com/FriendsOfSymfony/FOSJsRoutingBundle>`_
* Install jQuery. `SonatajQueryBundle <https://github.com/sonata-project/SonatajQueryBundle>`_ strongly suggested.

Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_tree_browser``

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_tree_browser:
            session:  default

Routing
-------

The bundle will create routes for each tree implementation found. In order to make 
those routes available you need to include the following in your routing configuration: 

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        symfony_cmf_tree:
            resource: .
            type: 'symfony_cmf_tree'

Usage
-----

You have select.js and init.js which are a wrapper to build a jquery tree. Use
them with SelectTree.initTree resp. AdminTree.initTree

 * SelectTree in select.js is a tree to select a node to put its id into a field
 * AdminTree in init.js is a tree to create, move and edit nodes

Both have the following options when creating:

 * config.selector: jquery selector where to hook in the js tree
 * config.rootNode: id to the root node of your tree, defaults to "/"
 * config.selected: id of the selected node
 * config.ajax.children_url: Url to the controller that provides the children of a node
 * config.routing_defaults: array for route parameters (such as _locale etc.) 
 * config.path.expanded: tree path where the tree should be expanded to at the moment
 * config.path.preloaded: tree path what node should be preloaded for faster user experience

select.js only
~~~~~~~~~~~~~~

 * config.output: where to write the id of the selected node

init.js only
~~~~~~~~~~~~

 * config.labels: array containing the translations for the labels of the context menu (keys 'createItem' and 'deleteItem')
 * config.ajax.move_url: Url to the controller for moving a child (i.e. giving it a new parent node)
 * config.ajax.reorder_url: Url to the controller for reordering siblings
 * config.types: array indexed with the node types containing information about valid_children, icons and available routes, used for the creation of context menus and checking during move operations.

Examples
--------

Look at the templates in the Sonata Admin Bundle for examples how to build the tree:

* `init.js <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/master/Resources/views/Tree/tree.html.twig>`_
* `select.js <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/master/Resources/views/Form/form_admin_fields.html.twig>`_ (look for doctrine_phpcr_type_tree_model_widget)

In the same bundle the `PhpcrOdmTree <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/master/Tree/PhpcrOdmTree.php>`_ implements the tree interface and gives an example how to implement the methods.
