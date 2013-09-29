.. index::
    single: Content; Bundles
    single: ContentBundle

ContentBundle
=============

    The ContentBundle provides a document for static content and the controller
    to render it.

For an introduction see the ":doc:`../../book/static_content`" article of the
"Book" section.

Installation
------------

You can install the bundle in 2 different ways:

* Use the official Git repository (https://github.com/symfony-cmf/ContentBundle);
* Install it via Composer (``symfony-cmf/content-bundle`` on `Packagist`_).

Usage
-----

The ContentBundle provides a ``StaticContent`` document which can be used for
static content. This document requires a title and body and can be connected
to multiple routes and menu items. A simple page can be::

    use Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent;
    use Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route;

    // retrieve the route root node
    $routeRoot = $documentManager->find(null, '/cms/routes');
    
    $route = new Route();
    $route->setPosition($routeRoot, 'hello');

    $documentManager->persist($route); // add the route

    // retrieve the content root node
    $contentRoot = $documentManager->find(null, 'content');

    $content = new StaticContent();
    $content->setTitle('Hello World');
    $content->setBody(...);
    $content->addRoute($route);
    $content->setParent($contentRoot);
    $content->setName('hello-world');

    $documentManager->persist($content); // add the content

    $documentManager->flush(); // save changes

This code adds a ``/hello`` route and a ``hello-world`` content document, both
connected to eachother. This means visiting ``/hello`` should give you the
content of ``hello-world``. But before that, the correct controller needs to
be configured.

The ContentController
~~~~~~~~~~~~~~~~~~~~~

The ContentBundle provides a ``ContentController``. This will handle all
incomming requests for static content. This can only be done when it's
configured correctly.

Create the Template
...................

In order to render the content, you need to create and configure a template.
The template will recieve a ``cmfMainContent`` variabele, which contains the
current ``Content`` document.

For instance, a very simple template looks like:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/MainBundle/Resources/views/Content/static.html.twig #}
        {% extends '::layout.html.twig' %}

        {% block title -%}
            {{ cmfMainContent.title }}
        {%- endblock %}

        {% block content -%}
            <h1>{{ cmfMainContent.title }}</h1>

            {{ cmfMainContent.body|raw }}
        {%- endblock %}

    .. code-block:: html+php

        <!-- src/Acme/MainBundle/Resources/views/Content/static.html.php -->
        <?php $view->extend('::layout.html.twig') ?>

        <?php $view['slots']->set('title', $cmfMainContent->getTitle()) ?>

        <?php $view['slots']->start('content') ?>
        <h1><?php echo $cmfMainContent->getTitle() ?></h1>

        <?php echo $cmfMainContent->getBody() ?>
        <?php $view['slots']->stop() ?>

Next, configure this template as the default template:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        # ...
        cmf_content:
            default_template: AcmeMainBundle:Content:static.html.twig

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            
            <!-- ... -->

            <config xmlns="http://cmf.symfony.com/schema/dic/content"
                default-template="AcmeMainBundle:Content:static.html.twig"
            />
        </container>

    .. code-block:: php

        // app/config/config.yml

        // ...
        $container->loadFromExtension('cmf_content', array(
            'default_template' => 'AcmeMainBundle:Content:static.html.twig',
        ));

Setting up the Routing
----------------------

The router needs to know that the route ``hello``, and all other routes
connect to a ``Content`` document, should be passed to the
``ContentController``. To configure this, use the
``cmf_routing.dynamic.templates_by_class`` configuration option:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        # ...
        cmf_routing:
            dynamic:
                templates_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent: AcmeMainBundle:Content:static.html.twig

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            
            <!-- ... -->

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic>
                    <template-by-class
                        class="Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent">
                        AcmeMainBundle:Content:static.html.twig
                    </template-by-class>
        </container>

    .. code-block:: php

        // app/config/config.yml

        // ...
        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                'templates_by_class' => array(
                    'Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent' => 'AcmeMainBundle:Content:static.html.twig',
                ),
            ),
        ));

.. tip::

    Learn more about the ``templates_by_class`` in
    :ref:`reference-config-routing-template_by_class`

Now everything is configured correctly, navigating to ``/hello`` results in a
page displaying your content.

SonataAdminBundle Integration
-----------------------------

The ContentBundle also provides an Admin class to enable creating, editing and
removing static content from the admin panel. To enable the admin, use the
``cmf_content.persistence.phpcr.use_sonata_admin`` setting.

.. _`Packagist`: https://packagist.org/packages/symfony-cmf/content-bundle
