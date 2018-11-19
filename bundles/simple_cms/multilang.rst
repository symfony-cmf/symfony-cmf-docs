.. index::
    single: Multi-Language; SimpleCmsBundle

Multi-Language Support
----------------------

.. include:: ../_partials/unmaintained.rst.inc

Setting the option ``add_locale_pattern`` to ``true`` in a
``Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page`` document will
result in prefixing the associated route with ``/{_locale}``. Using the native
translation capabilities of PHPCR ODM it is now possible to create different
versions of the document for each language that should be available on the
website.

For example::

    use Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page;

    // ...

    // pass add_locale_pattern as true to prefix the route pattern with /{_locale}
    $page = new Page(array('add_locale_pattern' => true));

    $page->setPosition($parent, 'hello-world');
    $page->setTitle('Hello World!');
    $page->setBody('Really interesting stuff...');
    $page->setLabel('Hello World');

    $dm->persist($page);
    $dm->bindTranslation($page, 'en');

    $page->setTitle('Hallo Welt!');
    $page->setBody('Super interessante Sachen...');
    $page->setLabel('Hallo Welt!');

    $dm->bindTranslation($page, 'de');

    $dm->flush();

Unless you write your own listener to update the locale *before* the routing
takes place, your content will be loaded in the default locale and you will
need to reload the content after the routing step to get the requested locale.

.. sidebar:: Translating the URL

    Since SimpleCmsBundle only provides a single tree structure, all nodes
    will have the same node name for all languages. So a url
    ``http://foo.com/en/hello-world`` for English content will look like
    ``http://foo.com/de/hello-world`` for German content.

    If you need language specific URLs, you can either add Route documents for
    the other locales and configure the dynamic router to look for routes under
    both prefixes, or contribute the feature described in the `issue tracker`_.
    You can also completely separate routing and content by using the separate
    documents from the RoutingBundle and ContentBundle.

.. _`issue tracker`: https://github.com/symfony-cmf/simple-cms-bundle/issues/109
