.. index::
    single: TreeBrowser; Bundles
    single: TreeBrowserBundle

TreeBrowserBundle
=================

    The TreeBrowserBundle provides a tree navigation on top of a PHPCR
    repository.

This bundle consists of two parts:

* Generic Tree Browser with a TreeInterface
* PHPCR tree implementation and GUI for a PHPCR browser

Installation
------------

You can install the bundle in 2 different ways:

* Use the official Git repository (https://github.com/symfony-cmf/TreeBrowserBundle);
* Install it via Composer (``symfony-cmf/tree-browser-bundle`` on `Packagist`_).

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
        <import resource="."
            type="cmf_tree" />

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import('.', 'cmf_tree'));

        return $collection;

Usage
-----

You have select.js and init.js which are a wrapper to build a jquery tree. Use
them with ``SelectTree.initTree`` resp. ``AdminTree.initTree``

* SelectTree in select.js is a tree to select a node to put its id into a
  field
* AdminTree in init.js is a tree to create, move and edit nodes

Both have the following options when creating:

* **config.selector**: jquery selector where to hook in the js tree
* **config.rootNode**: id to the root node of your tree, defaults to "/"
* **config.selected**: id of the selected node
* **config.ajax.children_url**: Url to the controller that provides the
  children of a node
* **config.routing_defaults**: array for route parameters (such as _locale
  etc.)
* **config.path.expanded**: tree path where the tree should be expanded to at
  the moment
* **config.path.preloaded**: tree path what node should be preloaded for
  faster user experience

select.js only
~~~~~~~~~~~~~~

* **config.output**: where to write the id of the selected node

init.js only
~~~~~~~~~~~~

* **config.labels**: array containing the translations for the labels of the
  context menu (keys ``createItem`` and ``deleteItem``)
* **config.ajax.move_url**: Url to the controller for moving a child (i.e.
  giving it a new parent node)
* **config.ajax.reorder_url**: Url to the controller for reordering siblings
* **config.types**: array indexed with the node types containing information
  about valid_children, icons and available routes, used for the creation of
  context menus and checking during move operations.

Examples
--------

Look at the templates in the Sonata Admin Bundle for examples how to build the
tree:

* `init.js`_
* `select.js`_ (look for ``doctrine_phpcr_type_tree_model_widget``)

In the same bundle the `PhpcrOdmTree`_ implements the tree interface and gives
an example how to implement the methods.

Here are some common tips about TreeBrowser utilization:

Define Tree Elements
~~~~~~~~~~~~~~~~~~~~

The first step, is to define all the elements allowed in the tree and their
children. Have a look at the `cmf-sandbox configuration`_, the section
``document_tree`` in ``sonata_doctrine_phpcr_admin``.

This configuration is set for all your application trees regardless their type
(admin or select).

.. configuration-block::

    .. code-block:: yaml

        sonata_doctrine_phpcr_admin:
            document_tree_defaults: [locale]
            document_tree:
                Doctrine\ODM\PHPCR\Document\Generic:
                    valid_children:
                        - all
                Symfony\Cmf\Bundle\ContentBundle\Document\MultilangStaticContent:
                    valid_children:
                        - Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\SimpleBlock
                        - Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ContainerBlock
                        - Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ReferenceBlock
                        - Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ActionBlock
                Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\ReferenceBlock:
                    valid_children: []
                # ...

How to add an Admin Tree to Your Page
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This can be done either in an action template or in a custom block.

You have to specify the tree root and the selected item, this allows you to
have different type of content in your tree.

In this example, we will have the menu elements.

For Symfony 2.2 and later:

.. configuration-block::

    .. code-block:: jinja

        {% render(controller('sonata.admin.doctrine_phpcr.tree_controller:treeAction')) with {
            'root':     websiteId ~ "/menu",
            'selected': menuNodeId,
            '_locale':  app.request.locale
        } %}

    .. code-block:: php

        <?php echo $view['actions']->render(new ControllerReference(
                'sonata.admin.doctrine_phpcr.tree_controller:treeAction',
                array(
                    'root'     => $websiteId.'/menu',
                    'selected' => $menuNodeId,
                    '_locale'  => $app->getRequest()->getLocale()
                ),
        )) ?>

For Symfony 2.1:

.. configuration-block::

    .. code-block:: jinja

        {% render 'sonata.admin.doctrine_phpcr.tree_controller:treeAction' with {
            'root':     websiteId ~ "/menu",
            'selected': menuNodeId,
            '_locale':  app.request.locale
        } %}

    .. code-block:: php

        <?php echo
        $view['actions']->render('sonata.admin.doctrine_phpcr.tree_controller:treeAction', array(
            'root'     => $websiteId.'/menu',
            'selected' => $menuNodeId,
            '_locale'  => $app->getRequest()->getLocale()
        )) ?>

How to Customize the Tree Behaviour
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The TreeBrowserBundle is based on `jsTree`_. jsTree works with events,
dispatched everytime the user does an action.

A simple way to customize the tree behavior is to bind your actions to those
events.

If you have a look at init.js and select.js, you will notice that actions are
already bound to some of the tree events. If the default behavior is not what
you need, jQuery provide the unbind function to solve the problem.

Here is a simple way to remove the context menu from the admin tree (add the
``controller`` call around the controller name inside ``render`` for Symfony
2.2):

.. configuration-block::

    .. code-block:: html+jinja

        {% render 'sonata.admin.doctrine_phpcr.tree_controller:treeAction' with {
            'root':     websiteId ~ "/menu",
            'selected': menuNodeId,
            '_locale':  app.request.locale
        } %}
        <script type="text/javascript">
            $(document).ready(function() {
                $('#tree').bind("before.jstree", function (e, data) {
                    if (data.plugin === "contextmenu") {
                        e.stopImmediatePropagation();
                        return false;
                    }
                });
            });
        </script>

    .. code-block:: html+php

        <?php
        $view['actions']->render('sonata.admin.doctrine_phpcr.tree_controller:treeAction', array(
            'root'     => $websiteId.'/menu',
            'selected' => $menuNodeId,
            '_locale'  => $app->getRequest()->getLocale()
        ))?>
        <script type="text/javascript">
            $(document).ready(function() {
                $('#tree').bind("before.jstree", function (e, data) {
                    if (data.plugin === "contextmenu") {
                        e.stopImmediatePropagation();
                        return false;
                    }
                });
            });
        </script>

By default, the item selection open the edit route of the admin class of the
element. This action is bind to the ``select_node.jstree``.

If you want to remove it, you just need to call the unbind function on this
event:

.. code-block:: html

    <script type="text/javascript">
        $(document).ready(function() {
            $('#tree').unbind('select_node.jstree');
        });
    </script>

Then you can bind it on another action.

For example, if your want to open a custom action:

.. configuration-block::

    .. code-block:: jinja

        $('#tree').bind("select_node.jstree", function (event, data) {
            if ((data.rslt.obj.attr("rel") == 'Symfony_Cmf_Bundle_MenuBundle_Document_MenuNode'
                || data.rslt.obj.attr("rel") == 'Symfony_Cmf_Bundle_MenuBundle_Document_MultilangMenuNode')
                && data.rslt.obj.attr("id") != '{{ menuNodeId }}'
            ) {
                var routing_defaults = {'locale': '{{ locale }}', '_locale': '{{ _locale }}'};
                routing_defaults["id"] = data.rslt.obj.attr("url_safe_id");
                window.location = Routing.generate('presta_cms_page_edit', routing_defaults);
            }
        });

    .. code-block:: php

        $('#tree').bind("select_node.jstree", function (event, data) {
            if ((data.rslt.obj.attr("rel") == 'Symfony_Cmf_Bundle_MenuBundle_Document_MenuNode'
                || data.rslt.obj.attr("rel") == 'Symfony_Cmf_Bundle_MenuBundle_Document_MultilangMenuNode')
                && data.rslt.obj.attr("id") != '<?php echo $menuNodeId ?>'
            ) {
                var routing_defaults = {'locale': '<?php echo $locale ?>', '_locale': '<?php echo $_locale ?>'};
                routing_defaults["id"] = data.rslt.obj.attr("url_safe_id");
                window.location = Routing.generate('presta_cms_page_edit', routing_defaults);
            }
        });

Don't forget to add your custom route to the
``fos_js_routing.routes_to_expose`` configuration:

.. configuration-block::

    .. code-block:: yaml

        fos_js_routing:
            routes_to_expose:
                - cmf_tree_browser.phpcr_children
                - cmf_tree_browser.phpcr_move
                - sonata.admin.doctrine_phpcr.phpcrodm_children
                - sonata.admin.doctrine_phpcr.phpcrodm_move
                - presta_cms_page_edit

    .. code-block:: xml

        <config xmlns="http://example.org/schema/dic/fos_js_routing">
            <routes-to-expose>cmf_tree_browser.phpcr_children</routes-to-expose>
            <routes-to-expose>cmf_tree_browser.phpcr_move</routes-to-expose>
            <routes-to-expose>sonata.admin.doctrine_phpcr.phpcrodm_children</routes-to-expose>
            <routes-to-expose>sonata.admin.doctrine_phpcr.phpcrodm_move</routes-to-expose>
            <routes-to-expose>presta_cms_page_edit</routes-to-expose>
        </config>

    .. code-block:: php

        $container->loadFromExtension('fos_js_routing', array(
            'routes_to_expose' => array(
                'cmf_tree_browser.phpcr_children',
                'cmf_tree_browser.phpcr_move',
                'sonata.admin.doctrine_phpcr.phpcrodm_children',
                'sonata.admin.doctrine_phpcr.phpcrodm_move',
                'presta_cms_page_edit',
            ),
        ));

.. _`Packagist`: https://packagist.org/packages/symfony-cmf/simple-cms-bundle
.. _`FOSJsRoutingBundle`: https://github.com/FriendsOfSymfony/FOSJsRoutingBundle
.. _`SonatajQueryBundle`: https://github.com/sonata-project/SonatajQueryBundle
.. _`init.js`: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/master/Resources/views/Tree/tree.html.twig
.. _`select.js`: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/master/Resources/views/Form/form_admin_fields.html.twig
.. _`PhpcrOdmTree`: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/blob/master/Tree/PhpcrOdmTree.php
.. _`cmf-sandbox configuration`: https://github.com/symfony-cmf/cmf-sandbox/blob/master/app/config/config.yml
.. _`jsTree`: http://www.jstree.com/documentation
