Symfony CMF Routing System
==========================

Concept
-------

Why a new routing mechanism?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

CMS are highly dynamic sites, where most of the content is managed by the
administrators rather than developers. The number of available pages can
easily reach the thousands, which is usually multiplied by the number of
available translations. Best accessibility and SEO practices, as well as user
preferences dictate that the URLs should be definable by the content managers.

The default Symfony2 routing mechanism, with its configuration file approach,
is not the best solution for this problem, as it's not suited to handle dynamic
user defined routes, nor it scales well to a large number of routes.

The solution
~~~~~~~~~~~~

In order to address these issues, a new routing system was developed, that
takes into account the typical needs of a CMS routing:

- User defined urls.
- Multi-site.
- Multi-language.
- Tree-like structure for easier management.
- Content, Menu and Route separation for added flexibility.

With these requirements in mind, the Symfony CMF Routing component was developed.

The ChainRouter
---------------

At the core of Symfony CMF's Routing component sits the ``ChainRouter``.
It's used as a replacement for Symfony2's default routing system and, like
it, is responsible for determining which Controller will handle each request.

The ``ChainRouter`` works by accepting a set of prioritized routing strategies,
`RouterInterface <http://api.symfony.com/2.1/Symfony/Component/Routing/RouterInterface.html>`_
implementations, commonly referred to as "Routers". The routers are reponsible
for matching an incoming request to an actual Controller, and to do so, the
``ChainRouter`` iterates over the configured Routers according to their configured
priority:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_routing_extra:
            chain:
                routers_by_id:
                    # enable the DynamicRouter with high priority to allow overwriting configured routes with content
                    symfony_cmf_routing_extra.dynamic_router: 200
                    
                    # enable the symfony default router with a lower priority
                    router.default: 100

You can also load Routers using tagged services, by using the `router` tag
 nd an optional `priority`. The higher the priority, the earlier your router
 will be asked to match the route. If you do not specify the priority, your
 router will come last. If there are several routers with the same priority,
 the order between them is undetermined. The tagged service will look like
 this:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_namespace.my_router:
                class: %my_namespace.my_router_class%
                tags:
                    - { name: router, priority: 300 }

    .. code-block:: xml

        <service id="my_namespace.my_router" class="%my_namespace.my_router_class%">
            <tag name="router" priority="300" />
            ..
        </service>

Currently, Symfony CMF comes with two default routers: ``DynamicRouter``
and Symfony2's default router.

The default Symfony2 router
---------------------------

Although it replaces the default routing mechanism, Symfony CMF Routing allows
you to keep using the existing system. In fact, the default routing is enabled
by default, so you can keep using the routes you declared in your configuration
files, or as declared by other bundles.

The DynamicRouter
-----------------

This Router can dynamically load Route instances from a given repository. It then
uses a matching process to the incoming requested url to a specific Route,
which in turn is used to determine which Controller to forward the request
to.

The bundle's default configuration states that ``DynamicRouter`` is disabled
by default. However, in Symfony CMF, it comes activated, as stated in the
configuration file:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_routing_extra:
            dynamic:
                enabled: true

This is the minimum configuration required to load the ``DynamicRouter`` as
a service, thus making it capable of performing any routing. Actually, when
you browse the default pages that come with the Symfony CMF SE, it's the
``DynamicRouter`` that's matching your requests with the Controllers and
Templates.

Getting the Route
~~~~~~~~~~~~~~~~~

The repository to use can be configured to best suit each implementation's
needs, and must implement the ``RouteRepositoryInterface``. As part of this
bundle, an implementation for `PHPCR-ODM <https://github.com/doctrine/phpcr-odm>`_
is provided, but you can easily create your own, as the Router itself is
storage agnostic. The default repository loads the route at the path in the
request and all parent paths to allow for some of the path segments being
parameters.

For more detailed information on this implementation and how you can customize
or extend it, refer to :doc:`../bundles/routing-extra`.

The ``DynamicRouter`` is able to match the incoming request to a Route object
from the underlying repository. The details on how this matching process
is carried out can be found in the :doc:`../components/routing`.

Getting the Controller and Template
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A Route needs to specify which Controller should handle a specific Request.
The ``DynamicRouter`` uses one of several possible methods to determine it
(in order of precedence):

- Explicit: The stored Route document itself can explicitly declare the target
    Controller ``getRouteDefaults()``.
- By alias: the Route returns a 'type' value in ``getRouteDefaults()``,
    which is then matched against the provided configuration from config.yml
- By class: requires the Route instance to implement ``RouteObjectInterface``
    and return an object for ``getRouteContent()``. The returned class type is
    then matched against the provided configuration from config.yml.
- Default: if configured, a default Controller will be used.

Appart from this, the ``DynamicRouter`` is also capable of dynamically specifying 
which Template will be used, in a similar way to the one used to determine
the Controller (in order of precedence):

- Explicit: The stored Route document itself can explicitly declare the target
    Template in ``getRouteDefaults()``.
- By class: requires the Route instance to implement ``RouteObjectInterface``
    and return an object for ``getRouteContent()``. The returned class type is
    then matched against the provided configuration from config.yml.


Here's an example on how to configure the above mentioned options:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_routing_extra:
            dynamic:
                generic_controller: symfony_cmf_content.controller:indexAction
                controllers_by_alias:
                    editablestatic: sandbox_main.controller:indexAction
                controllers_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: symfony_cmf_content.controller::indexAction
                templates_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: SymfonyCmfContentBundle:StaticContent:index.html.twig

Notice that ``enabled: true`` is no longer present. It's only required if
no other configuration paramenter is provided. The router is automatically
enabled as soon as you add any other configuration to the `dynamic` entry.

Linking a Route with a Model instance
-------------------------------------

Depending on you application's logic, a requested url may have an associated
model instance from the database. Those Routes can implement the ``RouteObjectInterface``,
and optionally return a model instance, that will be automatically passed
to the Controller as the ``$contentDocument`` variable.

Notice that a Route can implement the above mentioned interface but still
not to return any model instance, in which case no associated object will
be provided.

Furthermore, Routes that implement this interface can also have a custom Route
name, instead of the default Symfony core compatible name, and it can contain
any characters. This allows you, for example, to set a path as the route name.

Redirections
------------

You can build redirections by implementing the ``RedirectRouteInterface``.
If you are using the default ``PHPCR-ODM`` repository, a ready to use implementation
is provided in``RedirectRoute`` Document. It can redirect either to an absolute
URI, to a named Route that can be generated by any Router in the chain or
to another Route object in the repository. The actual redirection is handled
by a specific Controller, that can be configured like so:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_routing_extra:
            controllers_by_class:
                Symfony\Cmf\Component\Routing\RedirectRouteInterface:  symfony_cmf_routing_extra.redirect_controller:redirectAction

.. note::
    
    The actual configuration for this association exists as a service, not as part of
    a config.yml file. Like discussed before, any of the approaches can be used.

Url generation
--------------

Symfony CMF's Routing component uses the default Symfony2 components to handle
route generation, so you can use the default methods for generating your
urls, with a few added possibilities:

* Either pass an implementation of ``RouteObjectInterface`` as ``route`` parameter 
* Or pass a model instance as ``content`` parameter 
* Or supply an implementation of ``ContentRepositoryInterface`` and the id of the model instance as parameter ``content_id``

Integrating with SonataAdmin
----------------------------

If ``sonata-project/doctrine-phpcr-admin-bundle`` is added to the composer.json
require section, the route documents are exposed in the SonataDoctrinePhpcrAdminBundle.
For instructions on how to configure this Bundle see :doc:`../bundles/doctrine_phpcr_admin`.

By default, ``use_sonata_admin`` is automatically set based on whether
``SonataDoctrinePhpcrAdminBundle`` is available but you can explicitly disable it
to not have it even if sonata is enabled, or explicitly enable to get an error
if Sonata becomes unavailable.

You have a couple of configuration options for the admin. The ``content_basepath``
points to the root of your content documents.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_routing_extra:
            use_sonata_admin: auto # use true/false to force using / not using sonata admin
            content_basepath: ~ # used with sonata admin to manage content, defaults to symfony_cmf_core.content_basepath

Terms Form Type
---------------

The bundle defines a form type that can be used for classical "accept terms"
checkboxes where you place urls in the label. Simply specify
``symfony_cmf_routing_extra_terms_form_type`` as the form type name and specify
a label and an array with ``content_ids`` in the options

.. code-block:: php

    $form->add('terms', 'symfony_cmf_routing_extra_terms_form_type', array(
        'label' => 'I have seen the <a href="%team%">Team</a> and <a href="%more%">More</a> pages ...',
        'content_ids' => array('%team%' => '/cms/content/static/team', '%more%' => '/cms/content/static/more')
    ));

The form type automatically generates the routes for the specified content
and passes the routes to the trans twig helper for replacement in the label.

Further notes
-------------

For more information on the Routing component of Symfony CMF, please refer to:

- :doc:`../components/routing` for most of the actual functionality implementation 
- :doc:`../bundles/routing-extra` for Symfony2 integration bundle for Routing Bundle
- Symfony2's `Routing <http://symfony.com/doc/current/components/routing/introduction.html>`_ component page
