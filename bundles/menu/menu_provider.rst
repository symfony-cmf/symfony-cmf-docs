.. index::
    single: menu factory; MenuBundle

Menu Provider
=============

A menu provider is responsible for loading a menu when it is requested. KnpMenu
supports having several providers. The MenuBundle provides the
``PhpcrMenuProvider`` to load menu items from PHPCR-ODM.

Every menu is identified by the name of its root node under the path given by
the configuration key ``persistence.phpcr.menu_basepath``. The default path is
``/cms/menu``, so in the following docuemnt structure example ``main-menu`` and
``side-menu`` are both valid menu names:

.. code-block:: text

    ROOT
        cms:
            menu:
                main-menu:
                    item-1:
                    item-2:
                side-menu:
                    item-1:

You can use custom document classes for menu nodes if needed, as long as they
implement ``Knp\Menu\NodeInterface`` (so that they integrate with
KnpMenuBundle). The default ``MenuNode`` class discards children that do not
implement the ``Knp\Menu\NodeInterface``.

.. note::

    There is currently no support for Doctrine ORM or other persistence
    managers. This is not by design, but only because nobody built that yet.
    We would be glad for a pull request refactoring ``PhpcrMenuProvider`` into
    a base class suitable for all doctrine implementations, and storage
    specific providers.

You can also write your completely custom provider and register it with the
KnpMenu as explained in the `KnpMenuBundle custom provider documentation`_.

.. _`KnpMenuBundle custom provider documentation`: https://github.com/KnpLabs/KnpMenuBundle/blob/master/Resources/doc/custom_provider.md
