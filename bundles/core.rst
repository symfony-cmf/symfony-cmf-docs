CoreBundle
==========

This is the `CoreBundle <https://github.com/symfony-cmf/CoreBundle#readme>`_
for the Symfony2 content management framework. This bundle provides common functionality,
helpers and utilities for the other CMF bundles.

One of the provided features is an interface and implementation of a publish workflow checker
with an accompanying interface that models can implement that want to support this checker.

Furthermore it provides a twig helper exposing several useful functions for twig templates
to interact with PHPCR-ODM documents.

.. index:: CoreBundle, PHPCR, ODM, publish workflow

Configuration
-------------

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_core:
            document_manager: default
            role``: IS_AUTHENTICATED_ANONYMOUSLY # used by the publish workflow checker

Publish workflow checker
------------------------

The Bundle provides a ``symfony_cmf_core.publish_workflow_checker`` service which implements
``PublishWorkflowCheckerInterface``. This interface defines a single method ``checkIsPublished()``.

.. code-block:: php

    $publishWorkflowChecker = $container->get('symfony_cmf_core.publish_workflow_checker');
    $ignoreRole = false // if to ignore the role when deciding if to consider the document as published
    if ($publishWorkflowChecker->checkIsPublished($document, $ignoreRole)) {
        ..
    }

Twig extension
--------------

Implements the following functions:

* ``cmf_find``: find the document for the provided path and class
* ``cmf_is_published``: checks if the provided document is published
* ``cmf_prev``: returns the previous published document by examining the child nodes of the parent of the provided
* ``cmf_next``: returns the next published document by examining the child nodes of the parent of the provided
* ``cmf_children``: returns an array of all the children documents of the provided documents that are published
* ``cmf_document_locales``: gets the locales of the provided document

.. code-block:: jinja

    {% set page = cmf_find('/some/path') %}

    {% if cmf_is_published(page) %}
        {% set prev = cmf_prev(page) %}
        {% if prev %}
            <a href="{{ path(prev) }}">prev</a>
        {% endif %}

        {% set next = cmf_next(page) %}
        {% if next %}
            <span style="float: right; padding-right: 40px;"><a href="{{ path(next) }}">next</a></span>
        {%  endif %}

        {% for news in cmf_children(page)|reverse %}
            <li><a href="{{ path(news) }}">{{ news.title }}</a> ({{ news.publishStartDate | date('Y-m-d')  }})</li>
        {% endfor %}

        {% if 'de' in cmf_document_locales(page) %}
            <a href="{{ path(app.request.attributes.get('_route'), app.request.attributes.get('_route_params')|merge(app.request.query.all)|merge({'_locale': 'de'})) }}">DE</a>
        {%  endif %}
        {% if 'fr' in cmf_document_locales(page) %}
            <a href="{{ path(app.request.attributes.get('_route'), app.request.attributes.get('_route_params')|merge(app.request.query.all)|merge({'_locale': 'fr'})) }}">DE</a>
        {%  endif %}
    {%  endif %}
