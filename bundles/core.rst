The CoreBundle
==============

This is the `CoreBundle`_ for the Symfony2 content management framework. This
bundle provides common functionality, helpers and utilities for the other CMF
bundles.

One of the provided features is an interface and implementation of a publish
workflow checker with an accompanying interface that models can implement that
want to support this checker.

Furthermore it provides a twig helper exposing several useful functions for
twig templates to interact with PHPCR-ODM documents.

.. index:: CoreBundle, PHPCR, ODM, publish workflow

Configuration
-------------

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_core:
            document_manager: default
            role: IS_AUTHENTICATED_ANONYMOUSLY # used by the publish workflow checker

.. _bundle-core-publish_workflow:

Publish workflow checker
------------------------

The Bundle provides a ``symfony_cmf_core.publish_workflow_checker`` service
which implements ``PublishWorkflowCheckerInterface``. This interface defines a
single method ``checkIsPublished()``.

.. code-block:: php

    $publishWorkflowChecker = $container->get('symfony_cmf_core.publish_workflow_checker');

    // if to ignore the role when deciding if to consider the document as published
    $ignoreRole = false;
    
    if ($publishWorkflowChecker->checkIsPublished($document, $ignoreRole)) {
        // ...
    }

Dependency Injection Tags
-------------------------

cmf_request_aware
~~~~~~~~~~~~~~~~~

If you have services that need the request (e.g. for the publishing workflow
or current menu item voters), you can tag them with ``cmf_request_aware`` to
have a kernel listener inject the request. Any class used in such a tagged
service must have the ``setRequest`` method or you will get a fatal error::

    use Symfony\Component\HttpFoundation\Request;

    class MyClass
    {
        private $request;

        public function setRequest(Request $request)
        {
            $this->request = $request;
        }
    }

.. note::

    You should only use this tag on services that will be needed on every
    request. If you use this tag excessively you will run into performance
    issues. For seldom used services, you can inject the container in the
    service definition and call ``$this->container->get('request')`` in your
    code when you actually need the request.


Twig extension
--------------

Implements the following functions:

* **cmf_find**: returns the document for the provided path
* **cmf_find_many**: returns an array of documents for the provided paths
* **cmf_is_published**: checks if the provided document is published
* **cmf_prev**: returns the previous document by examining the child nodes of
  the provided parent
* **cmf_prev_linkable**: returns the previous linkable document by examining
  the child nodes of the provided parent
* **cmf_next**: returns the next document by examining the child nodes of the
  provided parent
* **cmf_next_linkable**: returns the next linkable document by examining the
  child nodes of the provided parent
* **cmf_child**: returns a child documents of the provided parent document and
  child node
* **cmf_children**: returns an array of all the children documents of the
  provided parent
* **cmf_linkable_children**: returns an array of all the linkable children
  documents of the provided parent
* **cmf_descendants**: returns an array of all descendants paths of the
  provided parent
* **cmf_document_locales**: gets the locales of the provided document
* **cmf_nodename**: returns the node name of the provided document
* **cmf_parent_path**: returns the parent path of the provided document
* **cmf_path**: returns the path of the provided document

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

.. _`CoreBundle`: https://github.com/symfony-cmf/CoreBundle#readme
