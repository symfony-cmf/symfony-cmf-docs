.. index::
    single: Extending Page; SimpleCmsBundle

Extending the Page class
------------------------

The default Page document (``Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page``)
is relatively simple, shipping with a handful of the most common properties
for building a typical page: title, body, tags, publish dates etc.

If this is not enough for your project you can easily provide your own
document by extending the default ``Page`` document and explicitly setting the
configuration parameter to your own document class:

.. configuration-block::

    .. code-block:: yaml

        cmf_simple_cms:
            persistence:
                phpcr:
                    document_class: Acme\DemoBundle\Document\MySuperPage

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/simplecms">
                <persistence>
                    <phpcr
                        document-class="Acme\DemoBundle\Document\MySuperPage"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_simple_cms', array(
            'persistence' => array(
                'phpcr' => array(
                    'document_class' => 'Acme\DemoBundle\Document\MySuperPage',
                ),
            ),
        ));

Alternatively, the default ``Page`` document contains an ``extras`` property.
This is a key - value store (where value must be string or ``null``) which can be
used for small trivial additions, without having to extend the default Page
document.

For example::

    use Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page;
    // ...

    $page = new Page();

    $page->setTitle('Hello World!');
    $page->setBody('Really interesting stuff...');
    $page->setLabel('Hello World');

    // set extras
    $extras = array(
        'subtext' => 'Add CMS functionality to applications built with the Symfony2 PHP framework.',
        'headline-icon' => 'exclamation.png',
    );

    $page->setExtras($extras);

    $documentManager->persist($page);

These properties can then be accessed in your controller or templates via the
``getExtras()`` or ``getExtra($key)`` methods.
