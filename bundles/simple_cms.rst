.. index::
    single: SimpleCms; Bundles
    single: SimpleCmsBundle

SimpleCmsBundle
===============

The `SimpleCmsBundle`_ provides a simplistic CMS on top of the CMF components
and bundles.

While the core CMF components focus on flexibility, the simple CMS trades away
some of that flexibility in favor of simplicity.

The SimpleCmsBundle provides a solution to easily map content, routes and menu
items based on a single tree structure in the content repository.

For a simple example installation of the bundle check out the
`Symfony CMF Standard Edition`_

You can find an introduction to the bundle in the `Getting started`_
section.

The `CMF website`_ is another application using the SimpleCmsBundle.

.. index:: SimpleCmsBundle, i18n

Dependencies
------------

As specified in the bundle ``composer.json`` this bundle depends on most CMF
bundles.

Configuration
-------------

The configuration key for this bundle is ``cmf_simple_cms``

The ``use_menu`` option automatically enables a service to provide menus out
of the simple cms if the MenuBundle is enabled. You can also explicitly
disable it if you have the menu bundle but do not want to use the default
service, or explicitly enable to get an error if the menu bundle becomes
unavailable.

The routing section is configuring what template or controller to use for a
content class. This is reusing what the cmf routing bundle does, please see
the corresponding :ref:`routing configuration section <bundle-routing-route-enhancer>`.
It also explains the ``generic_controller``.

See the section below for multilanguage support.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_simple_cms:
            persistence:
                phpcr:
                    enabled:              ~
                    basepath:             /cms/simple
                    manager_registry:     doctrine_phpcr
                    manager_name:         ~
                    document_class:       Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page
                    use_sonata_admin:     auto
                    sonata_admin:
                        sort:                 false
            use_menu:             auto
            routing:
                generic_controller:   cmf_content.controller:indexAction
                content_repository_id:  cmf_routing.content_repository
                uri_filter_regexp:
                controllers_by_alias:

                    # Prototype
                    alias:                []
                controllers_by_class:

                    # Prototype
                    alias:                []
                templates_by_class:

                    # Prototype
                    alias:                []


.. tip::

    If you have the Sonata PHPCR-ODM admin bundle enabled but do *NOT* want to
    show the default admin provided by this bundle, you can add the following
    to your configuration

    .. configuration-block::

        .. code-block:: yaml

            cmf_simple_cms:
                persistence:
                    phpcr:
                        sonata_admin: false

Multi-language support
----------------------

Setting ``addLocalePattern`` to true in the ``Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page``
document will result in prefixing the associated route with ``/{_locale}``. Using the native
translation capabilities of PHPCR ODM it is now possible to create different versions of the document
for each language that should be available on the website:

    $page = new Page();

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

.. note::

    Since SimpleCmsBundle only provides a single tree structure, all nodes
    will have the same node name for all languages. So a url
    ``http://foo.com/en/hello-world`` for english content will look like
    ``http://foo.com/de/hello-world`` for german content. At times it might be most
    feasible to use integers as the node names and simple append the title of
    the node in the given locale as an anchor. So for example
    ``http://foo.com/de/1#my title`` and ``http://foo.com/de/1#mein title``.
    If you need language specific URLs, you want to use the CMF routing bundle
    and content bundle directly to have a separate route document per
    language.

Rendering
---------

You can specify the template to render a SimpleCms page, or use a controller
where you then give the page document to the template. A simple example for
such a template is:

.. code-block:: jinja

    {% block content %}
        <h1>{{ page.title }}</h1>

        <div>{{ page.body|raw }}</div>

        <ul>
        {% for tag in page.tags %}
            <li>{{ tag }}</li>
        {% endfor %}
        </ul>
    {% endblock %}

If you have the CreateBundle enabled, you can also output the document with
RDFa annotations, allowing you to edit the content as well as the tags in the
frontend. The most simple form is the following twig block:

.. code-block:: jinja

    {% block content %}
        {% createphp page as="rdf" %}
            {{ rdf|raw }}
        {% endcreatephp %}
    {% endblock %}

If you want to control more detailed what should be shown with RDFa, see
chapter :doc:`create`.

Extending the Page class
------------------------

The default Page document ``Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page``
is relatively simple, shipping with a handful of the most common properties
for building a typical page: title, body, tags, publish dates etc.

If this is not enough for your project you can easily provide your own
document by extending the default Page document and explicitly setting the
configuration parameter to your own document class:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_simple_cms:
            persistence:
                phpcr:
                    document_class:       Acme\DemoBundle\Document\MySuperPage
                    # ...

Alternatively, the default Page document contains an ``extras`` property. This
is a key - value store (where value must be string or null) which can be used
for small trivial additions, without having to extend the default Page
document.

For example::

    $page = new Page();

    $page->setTitle('Hello World!');
    $page->setBody('Really interesting stuff...');

    // set extras
    $extras = array(
        'subtext' => 'Add CMS functionality to applications built with the Symfony2 PHP framework.',
        'headline-icon' => 'exclamation.png',
    );

    $page->setExtras($extras);

    $documentManager->persist($page);

These properties can then be accessed in your controller or templates via the
``getExtras()`` or ``getExtra($key)`` methods.

.. _`SimpleCmsBundle`: https://github.com/symfony-cmf/SimpleCmsBundle#readme
.. _`Symfony CMF Standard Edition`: https://github.com/symfony-cmf/symfony-cmf-standard
.. _`Getting started`: ../getting_started/simplecms
.. _`CMF website`: https://github.com/symfony-cmf/cmf-website/
