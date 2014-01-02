.. index::
    single: menu factory; MenuBundle

Menu Factory
============

The menu documents are only used for persisting the menu data, they are not
actually used when rendering a menu. ``MenuItem`` classes from the KnpMenu
component are the objects that are needed, a menu factory takes data provided by
a class implementing ``NodeInterface`` and creates a ``MenuItem`` tree.

.. _bundles_menu_menu_factory_url_generation:

URL Generation
--------------

A menu item should have a URL associated with it. The CMF provides the
``ContentAwareFactory``, which extends the KnpMenu ``RouterAwareFactory`` and
``MenuFactory``.

* The ``MenuFactory`` only supports using the ``uri`` option to specify the
  menu items link. 
* The ``RouterAwareFactory`` adds support for for generating a URL from the
  ``route`` and ``routeParameters`` options.
* The CMF adds the ``ContentAwareFactory`` which supports generating the URL
  from the ``content`` and ``routeParameters`` options when using the
  :ref:`dynamic router <bundles-routing-dynamic-generator>`.

The ``content`` option, if specified, must contain a class which implements
the ``RouteReferrersInterface``, see the :ref:`dynamic router
<bundles-routing-dynamic-generator>` documentation for more information.

URL generation is absolute or relative, depending on the boolean value of the
``routeAbsolute`` option.

.. _bundles_menu_menu_factory_link_type:

Link Type
---------

The ``linkType`` option is a CMF addition which enables you to specify which
of the three URL generation techniques to use.

The values for this options can be one of the following:

* ``null``: If the value is ``null`` (or the options is not set) then the link
  type is determined automatically by looking at each of the ``uri``, ``route`` and 
  ``content`` options and using the first one which is non-null.
* **uri**: Use the URI provided by the ``uri`` option.
* **route**: Generate a URL using the route named by the ``route`` option
  and the parameters provided by the ``routeParameters`` option.
* **content**: Generate a URL by passing the ``RouteReferrersInterface``
  instance provided by the ``content`` option to the router using any
  parameters provided by the ``routeParameters`` option.

Publish Workflow
----------------

The CMF menu factory also determines if menu nodes are published and therefore
visible by use of the :doc:`publish workflow checker
<../core/publish_workflow>`.

.. versionadded:: 1.1
    The ``MenuContentVoter`` was added in CmfMenuBundle 1.1.
    
The ``MenuContentVoter`` decides that a menu node is not published if the
content it is pointing to is not published.
