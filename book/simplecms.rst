.. index::
    single: SimpleCms

A Simple CMS
============

In most CMS use cases the most basic need is to associate content with a URL.
In the Symfony CMF, this is done by using a powerfull routing system, provided
by the RoutingBundle, and a ContentBundle. The RoutingBundle provides a
``Route`` object which can be associated with the ``Content`` object of the
ContentBundle.

Having two objects is the most flexible solution. You can have different
routes (e.g. per language) for the same content. Or you can organize your
content in a different logic than your URL tree. But in many situations,
having the route and the content be one and the same simplifies things. That
is exactly what the SimpleCmsBundle is doing. 

.. note::

    It's important to know that the SimpleCmsBundle is just a simple example
    how you can combine the CMF bundles into a complete CMS. Feel free to
    extend the SimpleCmsBundle or create your own bundle to do this task.

.. tip::

    To learn more about the menu, see ":doc:`structuring_content`".

Page Document
~~~~~~~~~~~~~

The SimpleCmsBundle provides an object called ``Page`` which implements both
``Route`` and ``Content`` objects and also a ``NodeInterface``, so you can use it in
your menu. This three-in-one approach is the key concept behind the bundle.

Creating a new Page
~~~~~~~~~~~~~~~~~~~

To create a page, use the
``Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page`` object (which
extends from ``Symfony\Cmf\Bundle\SimpleCmsBundle\Model\Page``)::

    use Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page;

    $page = new Page();
    $page->setTitle('About Symfony CMF');
    $page->setLabel('About');
    $page->setBody(...);

You can also set other things (e.g. tags).

All pages are stored in a simple tree structure. To set the position, use
``setPosition``. The first argument is the name for current page and the
second argument is the parent node. The name of the first argument is used as
the route. For instance, if you have this tree structure:

.. code-block:: text

    /cmf/simple/
        home/
        about/
        blog/
            symfony-cmf-is-great/

In this case, you have 4 pages: ``home``, ``about``, ``blog`` and
``symfony-cmf-is-great``. The page ``symfony-cmf-is-great`` is a child of
``blog`` and thus has the url ``/blog/symfony-cmf-is-great``. To create such a
structure, the code looks like this::

    use PHPCR\Util\NodeHelper;
    use Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page;

    // the PHPCR-ODM manager, more on that later
    $manager = ...;

    $root = NodeHelper::createPath($manager->getPhpcrSession(), '/cmf/simple');

    $home = new Page();
    // ... set up home
    $home->setPosition($root, 'home');

    $manager->persist($home); // add home to the database

    $about = new Page();
    // ... set up about
    $about->setPosition($root, 'about');

    $manager->persist($home); // add about to the database
    
    $blog = new Page();
    // ... set up blog
    $blog->setPosition($root, 'blog');

    $manager->persist($home); // add blog to the database

    $blogPost = new Page();
    // ... set up blog post
    $blogPost->setPostion($blog, 'symfony-cmf-is-great');

    $manager->persist($home); // add blog post to the database

    // as with all doctrine variants, the changes are only saved when the
    // flush method is called
    $manager->flush(); 

Every PHPCR-ODM document must have a parent document. Parents are never
created automatically, so we use the PHPCR NodeHelper to ensure we have
the root element (``/cmf/simple`` in this case).

.. note::

    The ``/cmf/simple`` basepath is actually already created by an
    :ref`initializer <phpcr-odm-repository-initializers>` of the
    SimpleCmsBundle.

Summary
-------

Congratulations! You are now able to create a simple web site using the
Symfony CMF. From here, each chapter will tell you a bit more about the CMF
and more about the things behind the SimpleCMSBundle. In the end, you'll be
able to create more advanced blog systems and other CMS websites.
