.. index::
    single: Routing; Components
    single: Routing

Routing
=======

    The Symfony CMF Routing component extends the Symfony core routing
    component to allow more flexibility. The most important difference is that
    the CMF Routing component can load routing information from a database.

.. tip::

    This chapter provides background information on the Symfony CMF routing
    implementation. If you simply want to use routing in the CMF, read the
    :doc:`../../bundles/routing/introduction`. If you want to customize how
    the routing works, please read on in this chapter.

Like the Symfony routing component, the CMF routing component does not require
the full Symfony Framework and can be used in standalone projects as well.

At the core of the Symfony CMF Routing component is the
:doc:`ChainRouter <chain>`. The ChainRouter tries to match a request with each
of its registered routers, ignoring the
:class:`Symfony\\Component\\Routing\\Exception\\ResourceNotFoundException`
until all routers got a chance to match. The first match wins - if no router
matched, the :class:`Symfony\\Component\\Routing\\Exception\\ResourceNotFoundException`
is thrown. The default Symfony router can be added to this chain, so the
standard routing mechanism can still be used in addition to any custom routing.

Additionally, this component provides the :doc:`DynamicRouter <dynamic>`. This
router is more configurable and flexible than the Symfony core router. It can
be configured to load routes from a database, dynamically add information to
the routes and also generate URLs from model classes.

The goal of Routing
-------------------

Routing is the task of a framework to determine, based on the web request, what
code it has to call and which parameters to apply. The Symfony core
:class:`Symfony\\Component\\Routing\\Matcher\\RequestMatcherInterface` defines
that a router must convert a :class:`Symfony\\Component\\HttpFoundation\\Request`
into an array of routing information. In the full stack Symfony framework, the
code to call is defined in the ``_controller`` field of the match parameters.
The framework is going to call the controller specified, matching eventual
parameters of that method by name with the other parameters found in the match
array or in the ``Request`` object attributes field.

.. note::

    For a good introduction to routing in the Symfony framework, please read
    the `Routing chapter of the Symfony book`_.

    To use this component outside of the Symfony framework context, have a
    look at the core Symfony `Routing Component`_ to get a fundamental
    understanding of the component. The CMF Routing Component just extends the
    basic behavior.

Installation
------------

You can install this component `with composer`_ using the
`symfony-cmf/routing`_ package. If you are using the
``symfony-cmf/routing-bundle`` you do not need to specify the component
separately, it is required automatically.

Symfony integration
-------------------

As mentioned before, this component was designed to use independently of the
Symfony framework.  However, if you wish to use it as part of your Symfony
CMF project, an integration bundle is also available. Read more about the
RoutingBundle in :doc:`../../bundles/routing/introduction` in the bundles
documentation.

Sections
--------

* :doc:`chain`
* :doc:`dynamic`
* :doc:`nested_matcher`

.. _`with composer`: http://getcomposer.org
.. _`symfony-cmf/routing`: https://packagist.org/packages/symfony-cmf/routing
.. _`Routing chapter of the Symfony book`: https://symfony.com/doc/current/routing.html
.. _`Routing Component`: https://symfony.com/doc/current/components/routing/introduction.html
