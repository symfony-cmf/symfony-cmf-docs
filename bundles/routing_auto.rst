RoutingAutoBundle
=================

The ``RoutingAutoBundle`` allows you to define automatically created routes
for documents. For example, a blog application has two routable contents, the
blog itself, and the posts for the blog. We will call these documents ``Blog`` and
``Post``, and we will class them as *content documents*.

If we create a new ``Blog`` with the title "My New Blog" the bundle could automatically
create the route ``/blogs/my-new-blog``. For each new ``Post`` it could create a route
such as ``/blogs/my-new-blog/my-posts-title``. This URL resolves to a special type of
route that we will call the *auto route*.

By default, when we update a content document that has an auto route the 
corresponding auto route will also be updated, when deleting a content document 
the corresponding auto route will also be deleted.

If required, the bundle can also be configured to do extra stuff, like, for example,
leaving a ``RedirectRoute`` when the location of a content document changes or
automatically displaying an index page when an unconfigured intermediate path is
accessed (for example, listing all the children unders ``/blogs`` instead of returning
a ``404``).

Anatomy of an automatic URL
---------------------------

The diagram below shows a fictional URL for a blog post. The first 6 elements
of the URL are what we will call the *content path*. The last element we will call
the *content name*.

.. image:: ../images/bundles/routing_auto_post_schema.png

The content path is further broken down into *route stacks* and *routes*. A route
stack is a group of routes and routes are simply documents in the PHPCR tree.

.. note::

   Normally each *route* in the URL will correspond to a PHPCR-ODM node of class ``Route``.
   At time of writing however this is not required, and nodes can simply be, for instance
   , ``Generic`` documents which are not routable and will result in a 404 if accessed
   directly.

Internally each route stack is provided by a *builder unit*. Builder units contain
a *path provider* and actions to take if the provided path exist or not. The goal
of each builder unit is to generate a path and then provide a route object for each
element in that path.

The configuration for the example above could be as follows:

.. code-block:: yaml

    routing_auto:
        
        auto_route_mapping:
            My\Namespace\Bundle\BlogBundle\Document\Post:
                content_path:
                    # corresponds first route stack in diagram: my, blog, my-blog
                    blog_path:
                        provider:
                            name: content_method
                            method: getBlogPath
                        exists_action:
                            strategy: use
                        not_exists_action:
                            strategy: throw_exception

                    # corresponds to second route stack: 2013,04,06
                    date:
                        provider:
                            name: content_method
                            method: getPublishedDateRouteNames
                        exists_action:
                            strategy: use
                        not_exists_action:
                            strategy: create

                # corresponds to the content name: My Post Title
                content_name:
                    provider:
                        name: content_method
                        method: getTitle
                    exists_action: 
                        strategy: auto_increment
                        pattern: -%d
                    not_exists_action: 
                        strategy: create


The ``Post`` document would then need to implement the methods named above as follows:

.. code-block:: php

    <?php
    
    class Post
    {
        public function getBlogRouteNames()
        {
            // we can return a string
            return 'path/to/my/blog'; // note the path is *not* absolute.
        }

        public function getPublishedDateRouteNames()
        {
            // or an array of route names
            return array('2013', '04', '06');
        }

        public function getTitle()
        {
            // or a normal string, by default it will be slugified/urlized
            return "My post title";
        }
    }

Path Providers
--------------

Path providers specify a target path which is used by the subsequent path actions provide
the actual route documents.

specified
~~~~~~~~~

This is the most basic path provider and allows you to specify an exact (fixed) path.

.. code-block:: yaml

    path_provider:
        name: specified
        path: this/is/a/path

Options:

 - ``path`` - **required** The path to provide. 

.. note::
   
    We do not never specifiy absolute URLs in the auto route system. A paths absoluteness is determined
    by its position in the builder unit chain. I.e. if the specified provider is first in the chain it
    will naturally be the base of an absolute URL.

content_method
~~~~~~~~~~~~~~

The ``content_method`` provider allows the content object (e.g. a blog ``Post``) to specify
a path using one of its methods.

Example 1:

.. code-block:: yaml

    path_provider:
        name: content_method
        method: getTitle

This example will use the existing method of ``Post`` to retrieve the title. By default
all strings are *slugified*. That is, "My post title" will be automatically changed to
"my-post-title".

Example 2:

.. code-block:: yaml

    path_provider:
        name: content_method
        method: getBlogPath
        slugify: false

This example uses the ``getBlogPath`` method of the post which has been added explicitly
for this purpose. It will return the URL to the blog, e.g. "my/blog".

The method can return the path either as a single string, a path or an array of path elements
as shown in the following example:

.. code-block:: php

    <?php

    class Post
    {
         public function getTitle()
         {
            return "This is a post";
         }

         public function getPath()
         {
            return "/this/is/a/path";
         }

         public function getPathElements()
         {
            return array('this', 'is', 'a', 'path');
         }
    }

Options:

 - ``method``: **required** Method used to return the route name / path / path elements.
 - ``slugify``: If we should use the slugifier, default is ``true``.
