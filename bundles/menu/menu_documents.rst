.. index::
    single: menu documents; MenuBundle

Menu Documents
==============

In accordance with the
:ref:`CMF bundle standards <contrib_bundles_baseandstandardimplementations>`
you are provided with two menu node implementations, a base document and a
standard document. Both exist in the ``Model`` namespace as storage agnostic
classes and in ``Doctrine\Phpcr`` with specific features for PHPCR-ODM.

Base Menu Node
--------------

The ``MenuNodeBase`` document implements only those features available in the
original KnpMenu node. Please refer to the "`Creating Menus: The Basics`_" section of
the KnpMenu component documentation for more information.

.. code-block:: php

    use Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\Menu;
    use Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\MenuNodeBase;

    // find the menu tree root
    $mainMenu = $dm->find(Menu::class, '/cms/menu/main');

    $node = new MenuNodeBase();
    $mainMenu->addChild($node);
    $node->setName('home');

    // Attributes are the HTML attributes of the DOM element representing the
    // menu node (e.g. <li/>)
    $node->setAttribute('attr_name', 'attr_value');
    $node->setAttributes(['attr_name', 'attr_value']);
    $node->setChildrenAttributes(['attr_name', 'attr_value']);

    // Display the node or not
    $node->setDisplay(true);
    $node->setDisplayChildren(true);

    // Any extra attributes you wish to associate with the menu node
    $node->setExtras(['extra_param_1' => 'value']);

    // The label and the HTML attributes of the label
    $node->setLabel('Menu Node');
    $node->setLabelAttributes(['style' => 'color: red;']);

    // The HTML attributes of the link (i.e. <a href=.../>
    $node->setLinkAttributes(['style' => 'color: yellow;']);

    // Specify a route name to use and the parameters to use with it
    $node->setRoute('my_hard_coded_route_name');
    $node->setRouteParameters([]);

    // Specify if the route should be rendered absolute (otherwise relative)
    $node->setRouteAbsolute(true);

    // Specify a URI
    $node->setUri('http://www.example.com');

The Standard Menu Node
----------------------

The standard menu node supports the following CMF specific features
out-of-the-box:

* Methods to set and get parent document;
* Content association;
* Link type specification (URI, route or content);
* Standard :doc:`publish workflow <../core/publish_workflow>` integration;
* Translation.

Content Association
~~~~~~~~~~~~~~~~~~~

In addition to sourcing the menu items link URL from an explicit URI or a
route object, the standard menu node provides a method to associate a
content document from which the URL can be generated::

    $menuItem = ...;
    $menuItem->setContent($content);

The content document can be any document which implements the
``RouteReferrersInterface``. See :ref:`bundles-routing-dynamic-generator`.

This content document will then be handled by the ``ContentExtension``, see
:ref:`URL Generation <bundles_menu_menu_factory_url_generation>` for more details.

Link Type Specification
~~~~~~~~~~~~~~~~~~~~~~~

The standard menu node supports specifying the menu items link type via the
``setLinkType`` method::

    $menuItem = ...;
    $menuItem->setLinkType('content');

See the :ref:`Menu Factory documentation <bundles_menu_menu_factory_link_type>` for
more information.

Translation
~~~~~~~~~~~

The standard menu node supports translation if enabled, allowing the
locale to be set via the ``setLocale`` method::

    $menuItem = ...;
    $menuItem->setLocale('fr');

See :ref:`Persisting Multi-Language Documents <bundles-core-multilang-persisting_multilang_documents>` for more details.

Publish Workflow
~~~~~~~~~~~~~~~~

The standard menu node implements ``PublishTimePeriodInterface`` and
``PublishableInterface``. Please refer to the
:doc:`publish workflow documentation <../core/publish_workflow>`.

.. versionadded:: 1.1
    The ``MenuContentVoter`` was added in CmfMenuBundle 1.1.

The ``MenuContentVoter`` decides that a menu node is not published if the
content it is pointing to is not published.

.. _`Creating Menus: The Basics`: https://github.com/KnpLabs/KnpMenu/blob/1.1.x/doc/01-Basic-Menus.markdown
