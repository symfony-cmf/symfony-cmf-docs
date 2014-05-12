.. index::
    single: Blog; Bundles
    single: BlogBundle

BlogBundle
==========

.. include:: _outdate-caution.rst.inc

.. include:: _not-stable-caution.rst.inc

This bundle aims to provide everything you need to create a full blog or
blog-like system. It also includes in-built support for the Sonata Admin
bundle to help you get up-and-running quickly.

Current features:

* Host multiple blogs within a single website;
* Place blogs anywhere within your route hierarchy;
* Sonata Admin integration.

Pending features:

* Full tag support;
* Frontend pagination (using knp-paginator);
* RSS/ATOM feed;
* Comments;
* User support (FOSUserBundle).

Notes on this document
----------------------

* The XML configuration examples may be formatted incorrectly.

Dependencies
------------

* :doc:`CmfRoutingBundle <routing/introduction>` is used to manage the routing;
* :doc:`CmfRoutingAutoBundle <routing_auto/introduction>` is used to manage automatically generate routes;
* :doc:`PHPCR-ODM <phpcr_odm/introduction>` is used to persist the bundles documents.

Configuration
-------------

Example:

.. configuration-block::

    .. code-block:: yaml

        # app/config.yml
        cmf_blog:
            use_sonata_admin: auto
            blog_basepath: /cms/blog
            class:
                blog_admin: Symfony\Cmf\Bundle\BlogBundle\Admin\BlogAdmin # Optional
                post_admin: Symfony\Cmf\Bundle\BlogBundle\Admin\PostAdmin # Optional
                blog: Symfony\Cmf\Bundle\BlogBundle\Document\Blog # Optional
                post: Symfony\Cmf\Bundle\BlogBundle\Document\Post # Optional

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://cmf.symfony.com/schema/dic/blog"
            use-sonata-admin="auto"
            blog-basepath="/cms/blog"
        >
            <class
                blog-admin="Symfony\Cmf\Bundle\BlogBundle\Admin\BlogAdmin"
                post-admin="Symfony\Cmf\Bundle\BlogBundle\Admin\PostAdmin"
                blog="Symfony\Cmf\Bundle\BlogBundle\Document\Blog"
                post="Symfony\Cmf\Bundle\BlogBundle\Document\Post"
            />
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_blog', array(
            'use_sonata_admin' => 'auto',
            'blog_basepath' => '/cms/blog',
            'class' => array(
                'blog_admin' => 'Symfony\Cmf\Bundle\BlogBundle\Admin\BlogAdmin', // optional
                'post_admin' => 'Symfony\Cmf\Bundle\BlogBundle\Admin\PostAdmin', // optional
                'blog' => 'Symfony\Cmf\Bundle\BlogBundle\Document\Blog', // optional
                'post' => 'Symfony\Cmf\Bundle\BlogBundle\Document\Post', // optional
            ),
        ));

Explanation:

* **use_sonata_admin** - Specify whether to attempt to integrate with sonata admin;
* **blog_basepath** - *required* Specify the path where the blog content should be placed when using sonata admin;
* **class** - Allows you to specify custom classes for sonata admin and documents;
  * **blog_admin**: FQN of the sonata admin class to use for managing ``Blog``'s;
  * **post_admin**: FQN of the sonata admin class to use for managing ``Post``'s;
  * **blog**: FQN of the document class that sonata admin will use for ``Blog``'s;
  * **post**: FQN of the document class that sonata admin will use for ``Post``'s.

.. note::

    If you change the default documents **it is necessary** to update the auto
    routing configuration, as the auto routing system will not recognize your new
    classes and consequently will not generate any routes.

Auto Routing
~~~~~~~~~~~~

The blog bundle uses the ``CmfRoutingAuto`` bundle to generate a route
for each content. You will need an auto routing configuration for this to work.

You can include the default in the main configuration file as follows:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            # ...
            - { resource: @CmfBlogBundle/Resources/config/routing/autoroute_default.yml }
        # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <!-- ... -->
            <import resource="@CmfBlogBundle/Resources/config/routing/autoroute_default" />
        </imports>
        <!-- ... -->

    .. code-block:: php

        // app/config/config.php
        $loader->import('config.php');
        // ...

The default configuration will produce URLs like the following::

    http://www.example.com/blogs/dtls-blog/2013-04-14/this-is-my-post

Refer to the :doc:`RoutingAutoBundle <routing_auto/introduction>` documentation
for more information.

Content Routing
~~~~~~~~~~~~~~~

To enable the routing system to automatically forward requests to the blog
controller when a ``Blog`` or ``Post``  content is associated with a route,
add the following under the ``controllers_by_class`` section of
``cmf_routing_extra`` in the main configuration file:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing_extra:
            # ...
            dynamic:
                # ...
                controllers_by_class:
                    # ...
                    Symfony\Cmf\Bundle\BlogBundle\Document\Blog: cmf_blog.blog_controller:listAction
                    Symfony\Cmf\Bundle\BlogBundle\Document\Post: cmf_blog.blog_controller:viewPostAction

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://cmf.symfony.com/schema/dic/blog">
            <dynamic>
                <controllers-by-class
                    class="Symfony\CmfBundle\BlogBundle\Document\Post"
                >
                    cmf_blog.blog_controller:listAction"
                </controllers-by-class>
            </dynamic>
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing_extra', array(
            // ...
            'dynamic' => array(
                'controllers_by_class' => array(
                    'Symfony\Cmf\Bundle\BlogBundle\Document\Blog' => 'cmf_blog.blog_controller:listAction',
                    'Symfony\Cmf\Bundle\BlogBundle\Document\Post' => 'cmf_blog.blog_controller:viewPostAction',
                ),
            ),
        ));

Sonata Admin
~~~~~~~~~~~~

The ``BlogBundle`` has admin services defined for Sonata Admin, to make the
blog system visible on your dashboard, add the following to the
``sonata_admin`` section:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            dashboard:
                groups:
                    # ...
                    blog:
                        label: blog
                        items:
                            - cmf_blog.admin
                            - cmf_post.admin

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://example.org/schema/dic/sonata_admin">
            <!-- ... -->

            <dashboard>
                <groups id="blog"
                    label="blog">
                    <item>cmf_blog.admin</item>
                    <item>cmf_post.admin</item>
                </groups>
            </dashboard>
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_admin', array(
            // ...
            'dashboard' => array(
                'groups' => array(
                    // ...
                    'blog' => array(
                        'label' => 'blog',
                        'items' => array(
                            'cmf_blog.admin',
                            'cmf_post.admin',
                        ),
                    ),
                ),
            ),
        ));

Tree Browser Bundle
~~~~~~~~~~~~~~~~~~~

If you use the Symfony CMF Tree Browser bundle you can expose the blog routes
to enable blog edition from the tree browser. Expose the routes in the
``fos_js_routing`` section of the configuration file:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        fos_js_routing:
            routes_to_expose:
                # ...
                - admin_bundle_blog_blog_create
                - admin_bundle_blog_blog_delete
                - admin_bundle_blog_blog_edit

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://example.org/schema/dic/fos_js_routing">
            <!-- ... -->
            <routes-to-expose>admin_bundle_blog_blog_create</routes-to-expose>
            <routes-to-expose>admin_bundle_blog_blog_delete</routes-to-expose>
            <routes-to-expose>admin_bundle_blog_blog_edit</routes-to-expose>
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('fos_js_routing', array(
            'routes_to_expose' => array(
                // ...
                'admin_bundle_blog_blog_create',
                'admin_bundle_blog_blog_delete',
                'admin_bundle_blog_blog_edit',
        )));

Integration
-----------

Templating
~~~~~~~~~~

The default templates are marked up for `Twitter Bootstrap`_. But it is easy
to completely customize the templates by **overriding** them.

The one template you will have to override is the default layout, you will
need to change it and make it extend your applications layout. The easiest way
to do this is to create the following file:

.. configuration-block::

    .. code-block:: jinja

        {# app/Resources/CmfBlogBundle/views/default_layout.html.twig #}
        {% extends "MyApplicationBundle::my_layout.html.twig" %}

        {% block content %}
        {% endblock %}

    .. code-block:: php

        <!-- app/Resources/CmfBlogBundle/views/default_layout.html.twig -->
        <?php $view->extend('MyApplicationBundle::my_layout.html.twig') ?>

        <?php $view['slots']->output('content') ?>

The blog will now use ``MyApplicationBundle::my_layout.html.twig`` instead of
``CmfBlogBundle::default_layout.html.twig``.

See `Overriding Bundle Templates`_ in the Symfony documentation for more
information.

.. _`controllers as services`: http://symfony.com/doc/current/cookbook/controller/service.html
.. _`Twitter Bootstrap`: http://twitter.github.com/bootstrap/
.. _`Overriding Bundle Templates`: http://symfony.com/doc/current/book/templating.html#overriding-bundle-templates
