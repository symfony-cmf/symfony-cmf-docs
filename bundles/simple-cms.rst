SimpleCmsBundle
===============

The `SimpleCmsBundle <https://github.com/symfony-cmf/SimpleCmsBundle#readme>`_
provides a simplistic CMS on top of the CMF components and bundles.

While the core CMF components focus on flexibility, the simple CMS trades
away some of that flexibility in favor of simplicity.

.. index:: SimpleCmsBundle, i18n

Dependencies
------------

As specified in the bundle ``composer.json`` this bundle depends on most CMF bundles.

Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_simple_cms``

The ``use_menu`` option automatically enables a service to provide menus out
of the simple cms if the MenuBundle is enabled. You can also explicitly
disable it if you have the menu bundle but do not want to use the default service,
or explicitly enable to get an error if the menu bundle becomes unavailable.

The routing section is configuring what template or controller to use for a
content class. This is reusing what routing extra does, please see the corresponding
:ref:`routing configuration section<routing-controller-mapper>`. It also explains the
``generic_controller``.

See the section below for multilanguage support.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_simple_cms:
            use_menu:             auto # use true/false to force providing / not providing a menu
            use_sonata_admin:     auto # use true/false to force using / not using sonata admin
            document_class:       Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page
            # controller to use to render documents with just custom template
            generic_controller:   symfony_cmf_content.controller:indexAction
            # where in the PHPCR tree to store the pages
            basepath:             /cms/simple
            routing:
                content_repository_id:  symfony_cmf_routing_extra.content_repository
                controllers_by_class:
                    # ...
                templates_by_class:
                    # ...
            multilang:
                locales:              []

.. Tip::

    If you have the Sonata PHPCR-ODM admin bundle enabled but do *NOT* want to
    show the default admin provided by this bundle, you can add the following
    to your configuration

    .. configuration-block::

        .. code-block:: yaml

            symfony_cmf_simple_cms:
                use_sonata_admin: false

Multi-language support
----------------------

The multi-language-mode is enabled by providing the list of allowed locales in
the ``multilang > locales`` field.

In multi-language-mode the Bundle will automatically use the
``Symfony\Cmf\Bundle\SimpleCmsBundle\Document\MultilangPage`` as the ``document_class``
unless a different class is configured explicitly.

This class will by default prefix all routes with ``/{_locale}``. This behavior
can be disabled by setting the second parameter in the constructor of the model to false.

Furthermore the routing layer will be configured to use
``Symfony\Cmf\Bundle\SimpleCmsBundle\Document\MultilangRouteRepository`` which will
ensure that even with the locale prefix the right content node will be found. Furthermore
it will automatically add a ``_locale`` requirement listing the current available locales
for the matched route.

.. Note::

    Since SimpleCmsBundle only provides a single tree structure, all nodes will have the same
    node name for all languages. So a url ``http://foo.com/en/bar`` for english content will
    look like ``http://foo.com/de/bar`` for german content. At times it might be most feasible
    to use integers as the node names and simple append the title of the node in the given locale
    as an anchor. So for example ``http://foo.com/de/1#my title`` and ``http://foo.com/de/1#mein title``.
    If you need language specific URLs, you want to use the CMF routing bundle and content bundle
    directly to have a separate route document per language.


Rendering
---------

You can specify the template to render a SimpleCms page, or use a controller
where you then give the page document to the template. A simple example for such
a template is


.. code-block:: jinja

    {% block content %}

        <h1>{{ page.title }}</h1>

        <div>{{ page.body|raw }}</div>

        <ul>
        {% foreach tag in page.tags %}
            <li>{{ tag }}</li>
        {% endforeach %}
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
