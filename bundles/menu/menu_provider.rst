.. index::
    single: menu factory; MenuBundle

Menu Provider
=============

A menu provider is responsible for loading a menu when it is requested. KnpMenu
supports having several providers. The MenuBundle provides the ``PhpcrMenuProvider``
to load menu items using the PHPCR-ODM.

Every menu is identified by the name of the node. These paths are relative to
the path configured by the ``cmf_menu.persistence.phpcr.menu_basepath``
setting. This defaults to ``/cmf/menu``. In order to get a menu located in
``/cms/menu/main-menu``, you can use the identifier ``main-menu``. You can also
use an absolute path to get menus that are not located in the menu basepath.

.. tip::

    You can use custom document classes for menu nodes if needed. But KnpMenu
    requires all nodes to implement ``Knp\Menu\NodeInterface``.

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
