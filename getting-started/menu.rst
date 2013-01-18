Menu
====

Concept
-------

No CMS system is complete without a menu system that allows users to navigate
between content pages and perform certain actions. While it does usually map
the actual content tree structure, menus often have a logic of their own,
include options not mapped by contents or exist in multiple contexts with
multiple options, thus making them a complex problem themselves.


Symfony CMF Menu System
-----------------------

Symfony CMF SE includes the ``MenuBundle``, a tool that allow you to dynamically
define your menus. It extends `KnpMenuBundle <https://github.com/knplabs/KnpMenuBundle>`_,
with a set of hierarchical, multilanguage menu elements, along with the tools
to load and store them from/to a database. It also includes the administration
panel definitions and related services needed for integration with
`SonataDoctrinePHPCRAdminBundle <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle>`_

.. note::

    The ``MenuBundle`` extends and greatly relies on `KnpMenuBundle <https://github.com/knplabs/KnpMenuBundle>`_,
    so you should carefully read `KnpMenuBundle's documentation <https://github.com/KnpLabs/KnpMenuBundle/blob/master/Resources/doc/index.md>`_.
    For the rest of this page we assume you have done so, and are familiar
    with concepts like Menu Providers and Menu Factories.


Usage
~~~~~

``MenuBundle`` uses ``KnpMenuBundle``'s default renderers and helpers to
print out menus. You can refer to the `respective documentation page <https://github.com/KnpLabs/KnpMenuBundle/blob/master/Resources/doc/index.md#rendering-menus>`_
for more information on the subject, but a basic call would be:

.. code-block:: jinja

    {{ knp_menu_render('simple') }}

The provided menu name will be passed on to ``MenuProviderInterface`` implementation,
which will use it to identify which menu you want rendered in this specific
section.


The Provider
~~~~~~~~~~~~

The core of ``MenuBundle`` is ``PHPCRMenuProvider``, a ``MenuProviderInterface``
implementation that's responsible for dynamically loading menus from a PHPCR
database. It does so based on the menu root's ``path`` value, by combining
a preconfigured ``basepath`` value with a ``name`` given by the developer
when instanciating the menu rendering call. This allows the ``PHPCRMenuProvider``
to handle several menu hierarchies using a single storage mechanism.

The menu element fetched using this process is used as the menu root node,
and its children will be loaded progressively as the full menu structure is
rendered by the ``MenuFactory``.


The Factory
~~~~~~~~~~~

The ``ContentAwareFactory`` is a ``FactoryInterface`` implementation, which
generates the full ``MenuItem`` hierarchy from the provided data. The data
generated this way is later used to generate the actual HTML representation
of the menu. 

The included implementation focuses on generating ``MenuItem`` instances
from ``NodeInterface`` instances, as it is the best approach to handle tree-like
structures like the ones typically used by CMS. Other approaches are implemented
in the extended classes, and their respective documentation pages can be found
in `KnpMenuBundle`_'s page.

``ContentAwareFactory`` is responsible for getting the full menu hierarchy
and rendering the respective ``MenuItem`` instances from the root node it
receives from the ``MenuProviderInterface`` implementation. It is also responsible
for determining which (if any) menu item is currently being viewed by the
user. ``KnpMenu`` already includes a specific factory targeted at Symfony2's
Routing component, which this bundle extends, to add support for:

- Databased stored ``Route`` instances (refer to :ref:`RoutingBundle's RouteProvider <routing-getting-route-object>` for more details
  on this)
- ``Route`` instances with associated content (more on this on respective :ref:`RoutingBundle's section <routing-linking-a-route-with-a-model-instance>`)

Like mentioned before, the ``ContentAwareFactory`` is responsible for loading
all the menu nodes from the provided root element. The actual loaded nodes
can be of any class, even if it's different from the root's, but all must
implement ``NodeInterface`` in order to be included in the generated menu.


The Menu Nodes
~~~~~~~~~~~~~~

Also included in ``MenuBundle`` come two menu node content types: ``MenuNode``
and ``MultilangMenuNode``. If you have read the documentation page regarding
:doc:`content`, you'll find this implementation somewhat familiar. ``MenuNode``
implements the above mentioned ``NodeInterface``, and holds the information
regarding a single menu entry: a ``label`` and a ``uri``, a ``children``
list, like you would expect, plus some ``attributes`` for himself and its
children, that will allow the actual rendering proccess to be customized.
It also includes a ``Route`` field and two references to Contents. These
are used to store an associated ``Route`` object, plus one (not two, despite
the fact that two fields exist) Content element. The ``MenuNode`` can have
a strong (integrity ensured) or weak (integrity not ensured) reference to
the actual Content element it points to, it's up to you to choose which best
fits your scenario. You can find more information on references on the
`Doctrine PHPCR documentation page <http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/association-mapping.html#references>`_.

``MultilangMenuNode`` extends ``MenuNode`` with multilanguage support. It
adds a ``locale`` field to identify which translation set it belongs to,
plus a ``label`` and ``uri`` fields marked as ``translated=true``, meaning
they will differ between translations, unlike the other fields.

It also specifies the strategy used to store the multiple translations to
database:

.. configuration-block::

    .. code-block:: php

       /**
       * @PHPCRODM\Document(translator="attribute")
       */

For information on the available translation strategies, refer to the Doctrine
page regarding `Multilanguage support in PHPCR-ODM <http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/multilang.html>`_

.. note::

    The ``MenuItem`` and ``MultilangMenuItem`` content types exist to preserve
    backwards compatibility with previous versions of the bundle, but they
    simply extend their Node counterparts. These classes are deprecated, and
    will be removed in a later version.

Admin support
-------------

``MenuBundle`` also includes the administration panels and respective services
needed for integration with `SonataDoctrinePHPCRAdminBundle <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle>`_,
a backoffice generation tool that can be installed with Symfony CMF. For
more information about it, please refer to the bundle's `documentation section <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/tree/master/Resources/doc>`_.

The included administration panels will automatically be loaded if you install
``SonataDoctrinePHPCRAdminBundle`` (refer to :doc:`../tutorials/creating-cms-using-cmf-and-sonata`
for instructions on how to do so).

Configuration
-------------

This bundle is configurable using a set of parameters, but all of them are
optional. You can go to the :doc:`../bundles/menu` reference page for the
full configuration options list and aditional information.

Further notes
-------------

For more information on the MenuBundle of Symfony CMF, please refer to:

- :doc:`../bundles/menu` for advanced details and configuration reference
- `KnpMenuBundle`_ page for information on the bundle on which ``MenuBundle`` relies 
- `KnpMenu <https://github.com/knplabs/KnpMenu>`_ page for information on the undelying library used by ``KnpMenuBundle``
