.. index::
    single: Core; Bundles
    single: CoreBundle

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
        cmf_core:
            document_manager_name: default
            role: IS_AUTHENTICATED_ANONYMOUSLY # used by the publish workflow checker

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!-- role: used by the publish workflow checker -->
        <config xmlns="http://symfony.com/schema/dic/core"
            document-manager-name="default"
            role="IS_AUTHENTICATED_ANONYMOUSLY"
        />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_core', array(
            'document_manager_name' => 'default',
            // used by the publish workflow checker
            'role'                  => 'IS_AUTHENTICATED_ANONYMOUSLY',
        ));

.. _bundle-core-publish_workflow:

Publish Workflow Checker
------------------------

The Bundle provides a ``cmf_core.publish_workflow_checker`` service
which implements ``PublishWorkflowCheckerInterface``. This interface defines a
single method ``checkIsPublished()``.

.. code-block:: php

    $publishWorkflowChecker = $container->get('cmf_core.publish_workflow_checker');

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

This tag is automatically translated to a `synchronized service`_ but as
Symfony 2.2 does not have that feature, you can use this tag for bundles that
you want to be able to use with Symfony 2.2. In custom applications that run
with Symfony 2.3, there is no need for this tag, just use the synchronized
service feature.

Templating
----------

Twig
~~~~

The bundle provides a Twig extension, implementing the following functions:

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
            <a href="{{ path(
                app.request.attributes.get('_route'),
                app.request.attributes.get('_route_params')|merge(app.request.query.all)|merge({
                    '_locale': 'de'
                })
            ) }}">DE</a>
        {%  endif %}
        {% if 'fr' in cmf_document_locales(page) %}
            <a href="{{ path(
                app.request.attributes.get('_route'),
                app.request.attributes.get('_route_params')|merge(app.request.query.all)|merge({
                    '_locale': 'fr'
                })
            ) }}">FR</a>
        {% endif %}
    {% endif %}

PHP
~~~

The bundle also provides a templating helper to use in PHP templates, it
contains the following methods:

* **find**: returns the document for the provided path
* **findMany**: returns an array of documents for the provided paths
* **isPublished**: checks if the provided document is published
* **getPrev**: returns the previous document by examining the child nodes of
  the provided parent
* **getPrevLinkable**: returns the previous linkable document by examining
  the child nodes of the provided parent
* **getNext**: returns the next document by examining the child nodes of the
  provided parent
* **getNextLinkable**: returns the next linkable document by examining the
  child nodes of the provided parent
* **getChild**: returns a child documents of the provided parent document and
  child node
* **getChildren**: returns an array of all the children documents of the
  provided parent
* **getLinkableChildren**: returns an array of all the linkable children
  documents of the provided parent
* **getDescendants**: returns an array of all descendants paths of the
  provided parent
* **getLocalesFor**: gets the locales of the provided document
* **getNodeName**: returns the node name of the provided document
* **getParentPath**: returns the parent path of the provided document
* **getPath**: returns the path of the provided document

.. code-block:: php

    <?php $page = $view['cmf']->find('/some/path') ?>

    <?php if $view['cmf']->isPublished($page) : ?>
        <?php $prev = $view['cmf']->getPrev($page) ?>
        <?php if ($prev) : ?>
            <a href="<?php echo $view['router']->generate($prev) ?>">prev</a>
        <?php endif ?>

        <?php $next = $view['cmf']->getNext($page) ?>
        <?php if ($next) : ?>
            <span style="float: right; padding-right: 40px;">
                <a href="<?php echo $view['router']->generate($next) ?>">next</a>
            </span>
        <?php  endif ?>

        <?php foreach (array_reverse($view['cmf']->getChildren($page)) as $news) : ?>
            <li>
                <a href="<?php echo $view['router']->generate($news) ?>"><?php echo $news->getTitle() ?></a>
                (<?php echo date('Y-m-d', $news->getPublishStartDate()) ?>)
            </li>
        <?php endforeach ?>

        <?php if (in_array('de', $view['cmf']->getLocalesFor($page))) : ?>
            <a href="<?php $view['router']->generate
                $app->getRequest()->attributes->get('_route'),
                array_merge(
                    $app->getRequest()->attributes->get('_route_params'),
                    array_merge(
                        $app->getRequest()->query->all(),
                        array('_locale' => 'de')
                    )
                )
            ?>">DE</a>
        <?php endif ?>
        <?php if (in_array('fr', $view['cmf']->getLocalesFor($page))) : ?>
            <a href="<?php $view['router']->generate
                $app->getRequest()->attributes->get('_route'),
                array_merge(
                    $app->getRequest()->attributes->get('_route_params'),
                    array_merge(
                        $app->getRequest()->query->all(),
                        array('_locale' => 'fr')
                    )
                )
            ?>">FR</a>
        <?php endif ?>
    <?php endif ?>

.. _`CoreBundle`: https://github.com/symfony-cmf/CoreBundle#readme
.. _`synchronized service`: http://symfony.com/doc/current/cookbook/service_container/scopes.html#using-a-synchronized-service
