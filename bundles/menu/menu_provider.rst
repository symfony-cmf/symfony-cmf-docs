.. index::
    single: menu factory; MenuBundle

Menu Provider
=============

A menu provider is responsible for loading a menu when it is requested. KnpMenu
supports having several providers. The CmfMenuBundle provides the
``PhpcrMenuProvider`` to load menu items using Doctrine PHPCR-ODM.

Every menu is identified by the name of the root node. The provider looks for
the root node under the the path configured in
``cmf_menu.persistence.phpcr.menu_basepath``. This defaults to ``/cmf/menu``.
In order to get a menu located at ``/cms/menu/main-menu``, you use the
identifier ``main-menu``. You can also use an absolute path to load menus that
are not located under the menu basepath.

.. tip::

    You can use custom document classes for menu nodes if needed, as long as
    they implement ``Knp\Menu\NodeInterface``.

.. note::

    There is currently no support for Doctrine ORM or other persistence
    managers. This is not by design, but only because nobody built that yet.
    We would be glad for a pull request refactoring ``PhpcrMenuProvider`` into
    a base class suitable for all doctrine implementations and storage specific
    providers.

.. seealso::

    You can also write your completely custom provider and register it with the
    KnpMenu as explained in the `KnpMenuBundle custom provider documentation`_.

.. _`KnpMenuBundle custom provider documentation`: https://symfony.com/doc/master/bundles/KnpMenuBundle/custom_provider.html
