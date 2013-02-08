BlogBundle
==========

This bundle aims to provides everything you need to create a full blog or
blog-like system. It also includes in-built support for the Sonata Admin
bundle to help you get up-and-running quickly.

Current features:

 - Host multiple blogs within a single website.
 - Place blogs anywhere within your route hierarchy.
 - Sonata Admin integration.

Pending features:

 - Full tag support
 - Frontend pagination (using knp-paginator)

Dependencies
------------

 * ``SymfonyCmfRoutingExtraBundle`` is used to manage the routing.
 * :doc:`PHPCR-ODM<phpcr-odm>` is used to persist the bundles documents.

Configuration
-------------

The default configuration will work with the ``cmf-sandbox``, you will probably
need to cusomize it to fit your own requirements:


.. code-block:: yaml

    symfony_cmf_blog:
        routing_post_controller: symfony_cmf_blog.blog_controller:viewPost
        routing_post_prefix: posts
        blog_basepath: /cms/content
        routing_basepath: /cms/routes

Explanation:

 * **routing_post_controller** - specifies which controller to use for showing posts.
   note that this controller MUST be a service, the method name follows.
 * **routing_post_prefix** - this is the part of the URL which "prefixes" the post slug
   e.g. with the default value the following post URL might be generated: ``http://example.com/my-blog/posts/this-is-my-post``
 * **blog_basepath** - *required* Specify the path where the blog content should be placed.
 * **routing_basepath** - *required* Specify the basepath for the routing system.


Routing
~~~~~~~

To enable the routing system to automatically forward requests to the blog
controller when a Blog content is associated with a route, add the following
under the ``controllers_by_class`` section of ``symfony_cmf_routing_extra``
in ``app/config/config.yml``:

.. code-block:: yaml

    symfony_cmf_routing_extra:
        ...
        dynamic:
            ...
            controllers_by_class:
                ...
                Symfony\Cmf\Bundle\BlogBundle\Document\Blog: symfony_cmf_blog.blog_controller:listAction

Sonata Admin
~~~~~~~~~~~~

The ``BlogBundle`` has admin services defined for Sonata Admin, to make the blog 
system visible on your dashboard, add the following to the 
``sonata_admin`` section:

.. code-block:: yaml

    sonata_admin:
        ...
        dashboard:
            groups:
                ...
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
            ...
            - admin_bundle_blog_blog_create
            - admin_bundle_blog_blog_delete
            - admin_bundle_blog_blog_edit
