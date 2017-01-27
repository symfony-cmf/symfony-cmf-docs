.. index::
    single: Rendering; SimpleCmsBundle

Rendering
---------

.. include:: ../_partials/unmaintained.rst.inc

You can configure a mapping of document class to template and/or controller
by configuring the :ref:`RoutingBundle <reference-config-routing-dynamic>`.
When you need specific settings for a single page, you can call
``setDefault()`` for the key ``_template`` or ``_controller`` default in the
page instance.

A simple example for such a template could look like this:

.. configuration-block::

    .. code-block:: html+jinja

        {% block content -%}
            <h1>{{ page.title }}</h1>

            <div>{{ page.body|raw }}</div>

            <ul>
                {% for tag in page.tags -%}
                    <li>{{ tag }}</li>
                {%- endfor %}
            </ul>
        {%- endblock %}

    .. code-block:: html+php

        <?php $view['slots']->start('content') ?>
        <h1><?php $page->getTitle() ?></h1>

        <div><?php $page->getBody() ?></div>

        <ul>
        <?php foreach ($page->getTags() as $tag) : ?>
            <li><?php echo $tag ?></li>
        <?php endforeach ?>
        </ul>
        <?php $view['slots']->stop() ?>

If you have the CreateBundle enabled, you can also output the document with
RDFa annotations, allowing you to edit the content as well as the tags in the
front-end. The most simple form is the following Twig block:

.. code-block:: jinja

    {% block content %}
        {% createphp page as="rdf" %}
            {{ rdf|raw }}
        {% endcreatephp %}
    {% endblock %}

If you want to control more detailed what should be shown with RDFa, see
chapter :ref:`bundle-create-usage-embed`.
