The BlogBundle
==============

This bundle aims to provide everything you need to create a full blog or
blog-like system. It also includes in-built support for the Sonata Admin
bundle to help you get up-and-running quickly.

Current features:

* Host multiple blogs within a single website;
* Place blogs anywhere within your route hierarchy;
* Sonata Admin integration.

Pending features:

* Full tag support
* Frontend pagination (using knp-paginator)
* RSS/ATOM feed
* Comments
* User support (FOSUserBundle)

Dependencies
------------

* :doc:`SymfonyCmfRoutingBundle <routing>` is used to manage the routing.
* :doc:`PHPCR-ODM <phpcr-odm>` is used to persist the bundles documents.

Configuration
-------------

Example:

.. code-block:: yaml

    # app/config.yml
    symfony_cmf_blog:
        use_sonata_admin:     auto
        blog_basepath:  /cms/blog
        class:
            blog_admin:           ~ # Required
            post_admin:           ~ # Required
            blog:                 ~ # Required
            post:                 ~ # Required

Explanation:

 * **use_sonata_admin** - Specify whether to attempt to integrate with sonata admin;
 * **blog_basepath** - *required* Specify the path where the blog content should be placed when using sonata admin;
 * **class** - Allows you to specify custom classes for sonata admin and documents;
   * **blog_admin**: FQN of the sonata admin class to use for managing ``Blog``'s;
   * **post_admin**: FQN of the sonata admin class to use for managing ``Post``'s;
   * **blog**: FQN of the document class that sonata admin will use for ``Blog``'s;
   * **post**: FQN of the document class that sonata admin will use for ``Post``'s.

.. note::

    You must either specify all the ``class`` keys or ommit the class key, you
    can however simply specify ``~`` (null) to use the default value.

Auto Routing
~~~~~~~~~~~~

The blog bundle uses the ``SymfonyCmfRoutingAuto`` bundle to generate a route
for each content. You will need an auto routing configuration for this to work.

You can include the default in ``config.yml`` as follows:

.. code-block:: yaml
    
    # app/config/config.yml
    imports:
        - { resource: parameters.yml }
        - { resource: security.yml }
        - { resource: @SymfonyCmfBlogBundle/Resources/config/routing/autoroute_default.yml }

The default configuration will produce URLs like the following::

    http://www.example.com/blogs/dtls-blog/2013-04-14/this-is-my-post

Refer to the :doc:`routing_auto` documentation for more information.

Content Routing
~~~~~~~~~~~~~~~

To enable the routing system to automatically forward requests to the blog
controller when a ``Blog`` or ``Post``  content is associated with a route,
add the following under the ``controllers_by_class`` section of
``symfony_cmf_routing`` in the config:

.. code-block:: yaml

    symfony_cmf_routing:
        # ...
        dynamic:
            # ...
            controllers_by_class:
                # ...
                Symfony\Cmf\Bundle\BlogBundle\Document\Blog: symfony_cmf_blog.blog_controller:listAction
                Symfony\Cmf\Bundle\BlogBundle\Document\Post: symfony_cmf_blog.blog_controller:viewPostAction

Sonata Admin
~~~~~~~~~~~~

The ``BlogBundle`` has admin services defined for Sonata Admin, to make the
blog system visible on your dashboard, add the following to the
``sonata_admin`` section:

.. code-block:: yaml

    # app/config/routing.yml
    sonata_admin:
        # ...
        dashboard:
            groups:
                # ...
                blog:
                    label: blog
                    items:
                        - symfony_cmf_blog.admin
                        - symfony_cmf_post.admin

Tree Browser Bundle
~~~~~~~~~~~~~~~~~~~

If you use the Symfony CMF Tree Browser bundle you can expose the blog routes
to enable blog edition from the tree browser. Expose the routes in the
``fos_js_routing`` section of ``app/config/config.yml``:

.. code-block:: yaml

    fos_js_routing:
        routes_to_expose:
            # ...
            - admin_bundle_blog_blog_create
            - admin_bundle_blog_blog_delete
            - admin_bundle_blog_blog_edit

Integration
-----------

Templating
~~~~~~~~~~

The default templates are marked up for `Twitter Bootstrap`_. But it is easy
to completely customize the templates by **overriding** them.

The one template you will have to override is the default layout, you will
need to change it and make it extend your applications layout. The easiest way
to do this is to create the following file:

.. code-block:: jinja

    {# /app/Resources/SymfonyCmfBlogBundle/views/default_layout.html.twig #}

    {% extends "MyApplicationBundle::my_layout.html.twig" %}

    {% block content %}
    {% endblock %}

The blog will now use ``MyApplicationBundle::my_layout.html.twig`` instead of
``SymfonyCmfBlogBundle::default_layout.html.twig``.

See `Overriding Bundle Templates`_ in the Symfony documentation for more
information.

.. _`controllers as services`: http://symfony.com/doc/current/cookbook/controller/service.html
.. _`Twitter Bootstrap`: http://twitter.github.com/bootstrap/
.. _`Overrding Bundle Templates`: http://symfony.com/doc/current/book/templating.html#overriding-bundle-templates
