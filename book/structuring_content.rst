.. index::
    single: Menu; Getting Started
    single: CmfMenuBundle

Structuring Content
===================

Menu Bundle
-----------

Concept
~~~~~~~

No CMS system is complete without a menu system that allows users to navigate
between content pages and perform certain actions. While it usually maps
the actual content tree structure, menus often have a logic of their own,
include options not mapped by content or exist in multiple contexts with
multiple options, thus making them a complex problem themselves.

Symfony CMF Menu System
~~~~~~~~~~~~~~~~~~~~~~~

Symfony CMF SE includes the MenuBundle, a tool that allow you to dynamically
define your menus. It extends the KnpMenuBundle_, with a set of
hierarchical, multi language menu elements, along with the tools to persist
them in the chosen content store. It also includes the administration panel
definitions and related services needed for integration with the
SonataDoctrinePHPCRAdminBundle_.

.. note::

    The MenuBundle extends and greatly relies on the KnpMenuBundle_, so you
    should carefully read `KnpMenuBundle's documentation`_. For the rest of
    this page we assume you have done so and are familiar with concepts like
    Menu Providers and Menu Factories.

Usage
.....

The MenuBundle uses KnpMenuBundle's default renderers and helpers to print out
menus. You can refer to the `respective documentation page`_ for more
information on the subject, but a basic call would be:

.. configuration-block::

    .. code-block:: jinja

        {{ knp_menu_render('simple') }}

    .. code-block:: php

        <?php echo $view['knp_menu']->render('simple') ?>

The provided menu name will be passed on to ``MenuProviderInterface``
implementation, which will use it to identify which menu you want rendered in
this specific section.

The Provider
............

The core of the MenuBundle is ``PhpcrMenuProvider``, a
``MenuProviderInterface`` implementation that's responsible for dynamically
loading menus from a PHPCR database. The default provider service is
configured with a ``menu_basepath`` to know where in the PHPCR tree it will
find menus. The menu ``name`` is given when rendering the menu and must be a
direct child of the menu base path. This allows the ``PhpcrMenuProvider`` to
handle several menu hierarchies using a single storage mechanism.

To give a concrete example, if we have the configuration as given below and
render the menu ``simple``, the menu root node must be stored at
``/cms/menu/simple``.

.. configuration-block::

    .. code-block:: yaml

        cmf_menu:
            menu_basepath: /cms/menu

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <container
            xmlns="http://symfony.com/schema/dic/services"
            xmlns:cmf-menu="http://cmf.symfony.com/schema/dic/menu">

            <cmf-menu:config menu-basepath="/cms/menu"/>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_menu', array(
            'menu_basepath' => '/cms/menu',
        ));

If you need multiple menu roots, you can create further ``PhpcrMenuProvider``
instances and register them with KnpMenu - see the CMF MenuBundle
``DependencyInjection`` code for the details.

The menu element fetched using this process is used as the menu root node, and
its children will be loaded progressively as the full menu structure is
rendered by the ``MenuFactory``.

The Factory
...........

The ``ContentAwareFactory`` is a ``FactoryInterface`` implementation, which
generates the full ``MenuItem`` hierarchy from the provided MenuNode. The data
generated this way is later used to generate the actual HTML representation of
the menu.

The included implementation focuses on generating ``MenuItem`` instances from
``NodeInterface`` instances, as this is usually the best approach to handle
tree-like structures typically used by a CMS. Other approaches are implemented in
the base classes, and their respective documentation pages can be found in
KnpMenuBundle_'s page.

``ContentAwareFactory`` is responsible for loading the full menu hierarchy and
transforming the ``MenuNode`` instances from the root node it receives from
the ``MenuProviderInterface`` implementation. It is also responsible for
determining which (if any) menu item is currently being viewed by the user.
It supports a voter mechanism to have custom code decide what menu item is
the current item.
``KnpMenu`` already includes a specific factory targeted at Symfony2's Routing
component, which this bundle extends, to add support for:

* ``Route`` instances stored in a database (refer to :ref:`RoutingBundle's
  RouteProvider <start-routing-getting-route-object>` for more details on
  this)
* ``Route`` instances with associated content (more on this on respective
  :ref:`RoutingBundle's section <start-routing-linking-a-route-with-a-model-instance>`)

As mentioned before, ``ContentAwareFactory`` is responsible for loading
all the menu nodes from the provided root element. The actual loaded nodes can
be of any class, even if it's different from the root's, but all must
implement ``NodeInterface`` in order to be included in the generated menu.

The Menu Nodes
..............

Also included in the MenuBundle are two menu node content types: ``MenuNode``
and ``MultilangMenuNode``. If you have read the documentation page regarding
:doc:`static_content`, you'll find this implementation somewhat familiar.

``MenuNode`` implements the above mentioned ``NodeInterface``, and holds the
information regarding a single menu entry: a ``label`` and a ``uri``, a
``children`` list, plus some ``attributes`` for the node
and its children that will allow the rendering process to be
customized. It also includes a ``Route`` field and two references to
Contents. These are used to store an associated ``Route`` object, plus one
(not two, despite the fact that two fields exist) Content element. The
``MenuNode`` can have a strong (integrity ensured) or weak (integrity not
ensured) reference to the actual Content element it points to; it's up to you
to choose which best fits your scenario. You can find more information on
references on the `Doctrine PHPCR documentation page`_.

``MultilangMenuNode`` extends ``MenuNode`` with multilanguage support. It adds
a ``locale`` field to identify which translation set it belongs to, plus
``label`` and ``uri`` fields marked as ``translated=true``. This means they
will differ between translations, unlike the other fields.

``MultilangMenuNode`` also specifies the strategy used to persist multiple
translations:

.. configuration-block::

    .. code-block:: php-annotations

        /**
         * @PHPCRODM\Document(translator="attribute")
         */

For information on the available translation strategies, refer to the Doctrine
page regarding `Multi language support in PHPCR-ODM`_

Admin Support
~~~~~~~~~~~~~

The MenuBundle also includes the administration panels and respective services
needed for integration with the backend admin tool
SonataDoctrinePHPCRAdminBundle_.

The included administration panels are automatically available but need to
be explicitly put on the dashboard if you want to use them. See
:doc:`../cookbook/creating_cms_using_cmf_and_sonata` for instructions on how
to install SonataDoctrinePHPCRAdminBundle.

Configuration
~~~~~~~~~~~~~

This bundle is configurable using a set of parameters, but all of them are
optional. You can go to the :doc:`../bundles/menu/index` reference page for the full
configuration options list and additional information.

.. _KnpMenuBundle: https://github.com/knplabs/KnpMenuBundle
.. _`KnpMenuBundle's documentation`: https://github.com/KnpLabs/KnpMenuBundle/blob/master/Resources/doc/index.md
.. _`respective documentation page`: https://github.com/KnpLabs/KnpMenuBundle/blob/master/Resources/doc/index.md#rendering-menus
.. _`Doctrine PHPCR documentation page`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/association-mapping.html#references
.. _`Multi language support in PHPCR-ODM`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/multilang.html
.. _`KnpMenu`: https://github.com/knplabs/KnpMenu
.. _SonataDoctrinePHPCRAdminBundle: http://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
