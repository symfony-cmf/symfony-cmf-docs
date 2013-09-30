.. index::
    single: Multi-Language; SimpleCmsBundle

Multi-Language Support
----------------------

Setting ``addLocalePattern`` to ``true`` in a
``Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page`` document will
result in prefixing the associated route with ``/{_locale}``. Using the native
translation capabilities of PHPCR ODM it is now possible to create different
versions of the document for each language that should be available on the
website.

For example::

    use Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page;

    // ...

    // pass true as the 3rd parameter to prefix the route pattern with /{_locale}
    $page = new Page(false, false, true);

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

.. sidebar:: Translating the URL

    Since SimpleCmsBundle only provides a single tree structure, all nodes
    will have the same node name for all languages. So a url
    ``http://foo.com/en/hello-world`` for english content will look like
    ``http://foo.com/de/hello-world`` for german content.
    
    At times it might be most feasible to use integers as the node names and
    simple append the title of the node in the given locale as an anchor. So
    for example ``http://foo.com/de/1#my title`` and
    ``http://foo.com/de/1#mein title``. If you need language specific URLs,
    you want to use the CMF RoutingBundle and ContentBundle directly to have
    a separate route document per language.
