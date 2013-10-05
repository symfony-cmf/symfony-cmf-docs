.. index::
    single: Templating; CoreBundle

Templating
----------

The CoreBundle also provides a lot of functions to use in templates.

Twig
~~~~

The CoreBundle contains a Twig extension that provides a set of useful
functions for your templates. The functions respect the
:doc:`publish workflow <publish_workflow>` if it is enabled.

* **cmf_find**: returns the document for the provided path
* **cmf_find_many**: returns an array of documents for the provided paths
* **cmf_is_published**: checks if the provided document is published, see
  :ref:`cmf_is_published <bundle-core-publish-workflow-twig_function>`.
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

An example of these functions can be:

.. code-block:: html+jinja

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

An example of these functions can be:

.. code-block:: html+php

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
        <?php endif ?>

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
