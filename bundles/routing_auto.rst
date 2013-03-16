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
stack is a group of routes provided by a special *path provider* class and routes
are simply documents in the PHPCR tree.

.. note::

   Normally each *route* in the URL will correspond to a PHPCR-ODM node of class ``Route``.
   At time of writing however this is not required, and nodes can simply be, for instance
   , ``Generic`` documents which are not routable and will result in a 404 if accessed
   directly.

The configuration for this URL could be as follows:

.. code-block:: yaml

    routing_auto:
        
        auto_route_mapping:
            My\Namespace\Bundle\BlogBundle\Document\Post:
                content_path:
                    # corresponds first route stack in diagram: my, blog, my-blog
                    blog_path:
                        path:
                            provider: content_method
                            method: getBlogRouteNames
                        exists:
                            strategy: use
                        not_exists:
                            strategy: throw_exception

                    # corresponds to second route stack: 2012,11,20
                    date:
                        path:
                            provider: content_method
                            method: getPublishedDateRouteNames
                        exists:
                            strategy: use
                        not_exists:
                            strategy: create

                # corresponds to the content name: My Post Title
                content_name:
                    strategy: content_method
                    method: getTitle
                    slugify: true

The ``Post`` document would then need to implement the methods named above as follows:

.. code-block:: php

    <?php
    
    class Post
    {
        public function getBlogRouteNames()
        {
            $blogPath = // determine the path of the blog
            $elements = // split the path elements

            return $elements; // e.g. array('blogs', 'my-blog');
        }

        public function getPublishedDateRouteNames()
        {
            $dateString = $this->getPublushedDate()->format('Y-m-d');
            $dateElements = explode('-', $dateString);
            return $dateElements;
        }

        public function getTitle()
        {
            return $this->title;
        }
    }

