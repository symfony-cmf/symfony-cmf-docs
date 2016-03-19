.. index::
    single: TreeBrowser; Bundles
    single: TreeBrowserBundle

TreeBrowserBundle
=================

    The TreeBrowserBundle provides a tree navigation on top of a PHPCR
    repository. The front-end implementation is based on the jQuery plugin
    `jsTree`_.

This bundle consists of two parts:

* Generic Tree Browser with a ``TreeInterface``
* PHPCR tree implementation and GUI for a PHPCR browser

Installation
------------

You can install this bundle `with composer`_ using the
``symfony-cmf/tree-browser-bundle`` package on `Packagist`_.

Both the CmfTreeBrowserBundle and FOSJsRoutingBundle_ must be registered in the
``AppKernel``::

    // app/appKernel.php
    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new FOS\JsRoutingBundle\FOSJsRoutingBundle(),
            new Symfony\Cmf\Bundle\TreeBrowserBundle\CmfTreeBrowserBundle(),
        );
        // ...

        return $bundles;
    }

Routing
-------

The bundle will create routes for each tree implementation found. In order to
make those routes available you need to include the following in your routing
configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        cmf_tree:
            resource: .
            type: 'cmf_tree'

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="." type="cmf_tree" />

        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import('.', 'cmf_tree'));

        return $collection;

Usage
-----

The TreeBrowserBundle provides the ``select_tree.js`` and ``admin_tree.js``
files, which are wrappers to build a jQuery tree. Use them with
``SelectTree.initTree`` resp. ``AdminTree.initTree``.

* ``SelectTree`` in ``select_tree.js`` is a tree to select a target tree node,
  e.g. in a form. The id of the target is put into a text field;
* ``AdminTree`` in ``admin_tree.js`` is a tree to create, move and edit nodes.

Both have the following options when creating:

* **config.selector**: jQuery selector where to hook in the js tree;
* **config.rootNode**: id to the root node of the tree, defaults to ``/``;
* **config.selected**: id of the selected node;
* **config.ajax.children_url**: URL to the controller that provides the
  children of a node;
* **config.routing_defaults**: array for route parameters (e.g. ``_locale``);
* **config.path.expanded**: tree path where the tree should be expanded to at
  the moment;
* **config.path.preloaded**: tree path what node should be preloaded for
  faster user experience.

select_tree.js only
~~~~~~~~~~~~~~~~~~~

* **config.output**: where to write the id of the selected node.

admin_tree.js only
~~~~~~~~~~~~~~~~~~

* **config.labels**: array containing the translations for the labels of the
  context menu (keys ``createItem`` and ``deleteItem``);
* **config.ajax.move_url**: Url to the controller for moving a child (i.e.
  giving it a new parent node);
* **config.ajax.reorder_url**: Url to the controller for reordering siblings;
* **config.types**: array indexed with the node types containing information
  about valid_children, icons and available routes, used for the creation of
  context menus and checking during move operations.

Examples
--------

Look at the templates in the SonataDoctrinePHPCRAdminBundle_ for examples how
you could build a tree:

* `admin_tree.js`_
* `select_tree.js`_ (look for ``doctrine_phpcr_type_tree_model_widget``)

In the same bundle, the `PhpcrOdmTree`_ implements the tree interface and
provides an example how to implement the methods.

Customizing the Tree Behavior
-----------------------------

The TreeBrowserBundle is based on `jsTree`_. jsTree works with events,
dispatched every time the user does an action. A simple way to customize the
tree behavior is to bind your actions to those events.

If you have a look at ``admin_tree.js`` and ``select_tree.js``, you will
notice that actions are already bound to some of the tree events. If the
default behavior is *not* what you need, jQuery provide the ``unbind`` method
to remove the default events. Here is a simple way to remove the context menu
from the admin tree:

.. configuration-block::

    .. code-block:: html+jinja

        {% render 'sonata.admin.doctrine_phpcr.tree_controller:treeAction' with {
            'root':     sitePath ~ "/menu",
            'selected': menuNodeId
        } %}
        <script type="text/javascript">
            $(document).ready(function() {
                $('#tree').bind("before.jstree", function (e, data) {
                    if ("contextmenu" === data.plugin) {
                        e.stopImmediatePropagation(); // stops executing of default event

                        return false;
                    }
                });
            });
        </script>

    .. code-block:: html+php

        <?php
        $view['actions']->render('sonata.admin.doctrine_phpcr.tree_controller:treeAction', array(
            'root'     => $sitePath . '/menu',
            'selected' => $menuNodeId,
        ))?>
        <script type="text/javascript">
            $(document).ready(function() {
                $('#tree').bind("before.jstree", function (e, data) {
                    if ("contextmenu" === data.plugin) {
                        e.stopImmediatePropagation(); // stops executing of default event

                        return false;
                    }
                });
            });
        </script>

.. note::

    This example assumes you have the SonataDoctrinePHPCRAdminBundle_
    available, to have a tree implementation.

By default, the item selection opens the edit route of the admin class of the
element. This action is bind to the ``select_node.jstree``. If you want to
remove it, you just need to call the unbind function on this event:

.. code-block:: javascript

    $(document).ready(function() {
        $('#tree').unbind('select_node.jstree');
    });

Then you can bind it on another action.

For example, if your want to open a custom action:

.. configuration-block::

    .. code-block:: jinja

        $('#tree').bind("select_node.jstree", function (event, data) {
            if ((data.rslt.obj.attr("rel") == 'Symfony_Cmf_Bundle_MenuBundle_Doctrine_Phpcr_MenuNode'
                && data.rslt.obj.attr("id") != '{{ menuNodeId }}'
            ) {
                var routing_defaults = {'locale': '{{ locale }}', '_locale': '{{ _locale }}'};
                routing_defaults["id"] = data.rslt.obj.attr("url_safe_id");
                window.location = Routing.generate('presta_cms_page_edit', routing_defaults);
            }
        });

    .. code-block:: php

        $('#tree').bind("select_node.jstree", function (event, data) {
            if ((data.rslt.obj.attr("rel") == 'Symfony_Cmf_Bundle_MenuBundle_Doctrine_Phpcr_MenuNode'
                && data.rslt.obj.attr("id") != '<?php echo $menuNodeId ?>'
            ) {
                var routing_defaults = {'locale': '<?php echo $locale ?>', '_locale': '<?php echo $_locale ?>'};
                routing_defaults["id"] = data.rslt.obj.attr("url_safe_id");
                window.location = Routing.generate('presta_cms_page_edit', routing_defaults);
            }
        });

.. note::

    This bundle automatically exposes routes with the FOSJsRoutingBundle_
    to allow the tree to work.

Read On
-------

* :doc:`configuration`

.. _`Packagist`: https://packagist.org/packages/symfony-cmf/tree-browser-bundle
.. _`with composer`: https://getcomposer.org
.. _`FOSJsRoutingBundle`: https://github.com/FriendsOfSymfony/FOSJsRoutingBundle
.. _`admin_tree.js`: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/1.2/Resources/views/Tree/tree.html.twig
.. _`select_tree.js`: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/1.2/Resources/views/Form/form_admin_fields.html.twig
.. _`PhpcrOdmTree`: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/1.2/Tree/PhpcrOdmTree.php
.. _`jsTree`: https://www.jstree.com/
.. _SonataDoctrinePHPCRAdminBundle: https://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
