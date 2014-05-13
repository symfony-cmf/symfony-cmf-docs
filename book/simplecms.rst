.. index::
    single: SimpleCms

First look at the internals
===========================

In most CMS use cases the most basic need is to associate content with a URL.
In the Symfony CMF, this is done by using a powerful routing system, provided
by the RoutingBundle, and the ContentBundle. The RoutingBundle provides a
``Route`` object which can be associated with the ``Content`` object of the
ContentBundle.

Having two objects is the most flexible solution. You can have different
routes (e.g. per language) for the same content. Or you can organize your
content in a differently than your URL tree. But in many situations,
having the route and the content be one and the same simplifies things. That
is exactly what the SimpleCmsBundle is doing, which is used by the Symfony
CMF Standard Edition by default for routing, content and menus.

.. note::

    It's important to know that the SimpleCmsBundle is just a simple example
    how you can combine the CMF bundles into a complete CMS. Feel free to
    extend the SimpleCmsBundle or create your own bundle to do this task.

.. tip::

    To learn more about the routing, see ":doc:`routing`". To learn more about
    content storage, see ":doc:`static_content`". Finally, to learn more about
    menus, see ":doc:`structuring_content`".

Page Document
~~~~~~~~~~~~~

The SimpleCmsBundle provides a class called ``Page`` which extends from the core
``Route`` class and provides properties to store content and also implements the
``NodeInterface``, so you can use inside the menu. This three-in-one approach is
the key concept behind the bundle.

The mapping of the ``Page`` to a template and controller works as explained in
the :doc:`previous chapter <routing>`.

Creating a new Page
~~~~~~~~~~~~~~~~~~~

To create a page, use the
``Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page`` object::

    // // src/Acme/MainBundle/DataFixtures/PHPCR/LoadSimpleCms.php
    namespace Acme\DemoBundle\DataFixtures\PHPCR;

    use Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page;
    use Doctrine\ODM\PHPCR\DocumentManager;

    class LoadSimpleCms implements FixtureInterface
    {
        /**
         * @param DocumentManager $dm
         */
        public function load(ObjectManager $dm)
        {
            $parent = $dm->find(null, '/cms/simple');
            $page = new Page();
            $page->setTitle('About Symfony CMF');
            $page->setLabel('About');
            $page->setBody(...);

            // the tree position defines the URL
            $page->setPosition($parent, 'about');

            $dm->persist($page);
            $dm->flush();
        }
    }

You can also set other options on the Page (e.g. tags).

All pages are stored in a simple tree structure. To set the position, use
``setPosition``. The first argument is the parent document, the second the
name for this page. The names are used for the URL. For instance, you may
have the following tree structure:

.. code-block:: text

    /cms/simple/
        about/
        blog/
            symfony-cmf-is-great/

In this case, you have 4 pages: the page at ``/cms/simple``, ``about``,
``blog`` and ``symfony-cmf-is-great``. The page at the home has the path
``/``. The page ``symfony-cmf-is-great`` is a child of ``blog`` and thus
has the path ``/blog/symfony-cmf-is-great``. To create such a
structure, you would do::


    // // src/Acme/MainBundle/DataFixtures/PHPCR/LoadSimpleCms.php
    namespace Acme\DemoBundle\DataFixtures\PHPCR;

    use Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page;
    use Doctrine\ODM\PHPCR\DocumentManager;

    class LoadSimpleCms implements FixtureInterface
    {
        /**
         * @param DocumentManager $dm
         */
        public function load(ObjectManager $dm)
        {
            $root = $dm->find(null, '/cms/simple');

            $about = new Page();
            // ... set up about
            $about->setPosition($root, 'about');

            $dm->persist($about);

            $blog = new Page();
            // ... set up blog
            $blog->setPosition($root, 'blog');

            $dm->persist($blog);

            $blogPost = new Page();
            // ... set up blog post
            $blogPost->setPosition($blog, 'symfony-cmf-is-great');

            $dm->persist($blogPost);

            $dm->flush();
        }
    }

Every PHPCR-ODM document must have a parent document. Parents are never
created automatically, so we use the PHPCR NodeHelper to ensure we have
the root element (``/cms/simple`` in this case).

.. note::

    The Page at ``/cms/simple`` is created by an
    :ref:`initializer <phpcr-odm-repository-initializers>` of the
    SimpleCmsBundle.

Summary
-------

Congratulations! You are now able to create a simple web site using the
Symfony CMF. From here, each chapter will tell you a bit more about the CMF
and more about the things behind the SimpleCMSBundle. In the end, you'll be
able to create more advanced blog systems and other CMS websites.
