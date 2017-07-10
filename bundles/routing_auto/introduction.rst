.. index::
    single: RoutingAuto; Bundles
    single: RoutingAutoBundle

RoutingAutoBundle
=================

The RoutingAutoBundle allows you to automatically persist routes when
documents are persisted based on URI schemas and contextual information.

This implies a separation of the ``Route`` and ``Content`` documents. If your
needs are simple this bundle may not be for you and you should have a look at
:doc:`the SimpleCmsBundle <../simple_cms/introduction>`.

Installation
------------

You can install this bundle `with composer`_ using the
`symfony-cmf/routing-auto-bundle`_ package.

This bundle is based on the :doc:`../routing/index`, and you need to
instantiate both bundles::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Symfony\Cmf\Bundle\RoutingBundle\CmfRoutingBundle(),
                new Symfony\Cmf\Bundle\RoutingAutoBundle\CmfRoutingAutoBundle(),
            );

            // ...
        }

        // ...
    }

Features
--------

Imagine you are going to create a forum application that has two routeable
content documents - a category and the topics. These documents are called
``Category`` and ``Topic``, and they are called *content documents*.

If you create a new category with the title "My New Category", the
RoutingAutoBundle will automatically create the route
``/forum/my-new-cateogry``. For each new ``Topic`` it could create a route
like ``/forum/my-new-category/my-new-topic``. This URI resolves to a special
type of route that is called an *auto route*.

By default, when you update a content document that has an auto route, the
corresponding auto route will also be updated. When deleting a content
document, the corresponding auto route will also be deleted.

If required, the bundle can also be configured to do extra stuff, like, for
example, leaving a ``RedirectRoute`` when the location of a content document
changes or automatically displaying an index page when an unconfigured
intermediate path is accessed (for example, listing all the children when requesting
``/forum`` instead of returning a ``404``).

Why not Simply Use a Single Route?
----------------------------------

Of course, our fictional forum application could use a single route with a
pattern ``/forum/my-new-forum/{topic}``, which could be handled by a controller.
Why not just do that?

#. By having a route for each page in the system, the application has a
   knowledge of which URIs are accessible. This can be very useful, for
   example, when specifying endpoints for menu items that are used when generating
   a site map;
#. By separating the route from the content you allow the route to be
   customized independently of the content, for example, a topic may have
   the same title as another topic but might need a different URI;
#. Separate route documents are translatable - this means you can have a URI
   for *each language*, ``/welcome`` and ``/bienvenue`` would each reference the
   same document in English and French respectively. This would be difficult
   if the slug was embedded in the content document;
#. By decoupling route and content the application doesn't care *what* is
   referenced in the route document. This means that you can easily replace the
   class of the document referenced.

Usage
-----

Imagine you have a fictional forum application and that you want to access the
forum topic with the following fictional URI:

- ``https://mywebsite.com/my-forum/drinks/coffee``

The RoutingAutoBundle uses a URI schema definitions to define how routes are generated. A
schema for the above URI would look like this (the bundle does not care about
the host or protocol):

- ``/my-forum/{category}/{title}``

You can see that ``my-forum`` is static (it will not change) but that
``drinks`` has been replaced with ``{category}`` and ``coffee`` with
``{title}``. The replacements are called *tokens*.

The value for tokens are provided by *token providers*.

The schema definitions, token providers, and other configurations (more on this later) are
contained within ``routing_auto.format`` files (currently ``xml`` and ``yaml`` are
supported). These files are contained either in your bundles
``Resources/config`` directory or in a custom location specified in
the bundle configuration.

The configuration files for the above schema as applied to a ``Topic``
document could be defined as follows:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/ForumBundle/Resources/config/cmf_routing_auto.yml
        Acme\ForumBundle\Document\Topic:
            definitions:
                 main:
                     uri_schema: /my-forum/{category}/{title}
            token_providers:
                category: [content_method, { method: getCategoryTitle, slugify: true }]
                title: [content_method, { method: getTitle }] # slugify is true by default

    .. code-block:: xml

        <!-- src/Acme/ForumBundle/Resources/config/cmf_routing_auto.xml -->
        <?xml version="1.0" ?>
        <auto-mapping xmlns="http://cmf.symfony.com/schema/routing_auto">
            <mapping class="Acme\ForumBundle\Document\Topic">
                <definition name="main" uri-schema="/my-forum/{category}/{title}" />

                <token-provider token="category" name="content_method">
                    <option name="method">getCategoryName</option>
                    <option name="slugify">true</option>
                </token-provider>

                <token-provider token="title" name="content_method">
                    <option name="method">getTitle</option>
                </token-provider>
            </mapping>
        </auto-mapping>

The ``Topic`` document would then need to implement the methods named above as
follows::

    // src/Acme/ForumBundle/Document/Topic.php
    namespace Acme\ForumBundle\Document;

    class Topic
    {
        /**
         * Returns the category object associated with the topic.
         */
        public function getCategoryName()
        {
            return 'Drinks';
        }

        public function getTitle()
        {
            return 'Coffee';
        }
    }

After persisting this object, the route will be created. You will of course
be wanting to return property values and not static strings, but you get the
idea.

.. note::

    Any mapping applied to an object will also apply to subclasses of that
    object. Imagine you have 2 documents, ``ContactPage`` and ``Page``, which
    both extend ``AbstractPage``. When you map the ``AbstractPage`` class, it
    will be applied to both documents. You can also use the ``extend`` keyword
    to achieve the same thing with objects which are not related.

This is just a basic example. You can also configure what should happen when
a route already exists (conflict resolution) and what to do with old routes
when the generated URI is changed (defunct route handling).

Read more
---------

* :doc:`token_providers`
* :doc:`conflict_resolvers`
* :doc:`defunct_route_handlers`
* :doc:`definitions`
* :doc:`adapter`
* :doc:`configuration`
* :doc:`Sonata Admin integration <../sonata_phpcr_admin_integration/routing_auto>`

.. _`with composer`: https://getcomposer.org/
.. _`symfony-cmf/routing-auto-bundle`: https:/packagist.org/packages/symfony-cmf/routing-auto-bundle
