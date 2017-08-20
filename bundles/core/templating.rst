.. index::
    single: Templating; CoreBundle

Templating
----------

The CoreBundle provides a number of functions to use in templates.

Twig and PHP Helper
~~~~~~~~~~~~~~~~~~~

The CoreBundle registers a Twig extension and a PHP templating helper. Both
work the same way and are documented together. We always list both the Twig
function name and the ``cmf`` helper method names.

Many methods share the same common parameters:

* **ignoreRole**: Methods having this parameter by default filter the
  result set to only contain published documents, using
  :doc:`publish workflow <publish_workflow>`. You may set the parameter to
  ``true`` to disable the filtering if you know what you are doing.
* **class**: The class parameter can be used to filter the result set by
  removing all documents that do not implement the specified class.
* **limit, offset**: Used to slice the result list for paging. Note that
  slicing happens before the filtering.

Basic repository operations
...........................

* **cmf_find|find(path)**: Get the document for the provided path or null if not found.
  No publish workflow check is done.
* **cmf_find_translation|findTranslation($pathOrDocument, $locale, $fallback = true)**: Get the document translation
  for the provided path/document and locale or null if not found. The document will stay in the last requested locale
  for the rest of the request.
  No publish workflow check is done.
* **cmf_find_many|findMany($paths, $limit = false, $offset = false, $ignoreRole = false, $class = false)**:
  Get a list of documents for the provided paths.
* **cmf_nodename|getNodeName($document)**: Get the node name of the provided document.
* **cmf_path|getPath($document)**: Get the path of the provided document.
* **cmf_parent_path|getParentPath($document)**: Get the path of the parent of the provided document.

.. versionadded:: 1.3.1
    The cmf_find_translation twig function has been added in version 1.3.1 of the CMF CoreBundle.

Walking the PHPCR tree
......................

+----------------------------+--------------------------+--------------------------+---------------------------------------------------------------------------+
| Twig Function              | Templating Helper        | Arguments                | Explanation                                                               |
+============================+==========================+==========================+===========================================================================+
| ``cmf_prev``               | ``getPrev``              | ``$current``,            | Get the previous sibling document of ``$current`` (a document or a path)  |
|                            |                          | ``$depth = null``,       | in PHPCR order. If ``$anchor`` (also a document or a path) is set, also   |
|                            |                          | ``$ignoreRole = false``  | walks up the tree to find neighbors of ``$current``. If ``$depth`` is     |
|                            |                          | ``$class = null``,       | set, this limits how deep below ``$current`` the tree is walked.          |
|                            |                          | ``$anchor = null``       |                                                                           |
+----------------------------+--------------------------+--------------------------+---------------------------------------------------------------------------+
| ``cmf_prev_linkable``      | ``getPrevLinkable``      | ``$current``,            | Get the previous document that can be linked to, according to the         |
|                            |                          | ``$anchor = null``,      | ``isLinkable`` method below.                                              |
|                            |                          | ``$depth = null``,       |                                                                           |
|                            |                          | ``$ignoreRole = false``  |                                                                           |
+----------------------------+--------------------------+--------------------------+---------------------------------------------------------------------------+
| ``cmf_next``               | ``getNext``              | ``$current``,            | Get the next sibling document from ``$current`` (a document or a path)    |
|                            |                          | ``$anchor = null``,      | in PHPCR order. ``$anchor`` and ``$depth`` have the same semantics as in  |
|                            |                          | ``$depth = null``,       | ``getPrev``.                                                              |
|                            |                          | ``$ignoreRole = false``  ,                                                                           |
|                            |                          | ``$class = null``        |                                                                           |
+----------------------------+--------------------------+--------------------------+---------------------------------------------------------------------------+
| ``cmf_next_linkable``      | ``getNextLinkable``      | ``$current``,            | Get the next document that can be linked to, according to the             |
|                            |                          | ``$anchor = null``,      | ``isLinkable`` method below.                                              |
|                            |                          | ``$depth = null``,       |                                                                           |
|                            |                          | ``$ignoreRole = false``  |                                                                           |
+----------------------------+--------------------------+--------------------------+---------------------------------------------------------------------------+
| ``cmf_child``              | ``getChild``             | ``$parent, $name``       | Get child document named ``$name`` of the specified parent. The parent    |
|                            |                          |                          | can be either the id or a document. No publish workflow check is done.    |
+----------------------------+--------------------------+--------------------------+---------------------------------------------------------------------------+
| ``cmf_children``           | ``getChildren``          | ``$parent``,             | Get all children of that parent in the order they appear in PHPCR. The    |
|                            |                          | ``$limit = false``,      | parent can be either the id or a document.                                |
|                            |                          | ``$offset = false``,     | *$filter is currently ignored*.                                           |
|                            |                          | ``$filter = null``,      |                                                                           |
|                            |                          | ``$ignoreRole = false``, |                                                                           |
|                            |                          | ``$class = null``        |                                                                           |
+----------------------------+--------------------------+--------------------------+---------------------------------------------------------------------------+
| ``cmf_linkable_children``  | ``getLinkableChildren``  | ``$parent``,             | Get all children of ``$parent`` that can be linked to, according to the   |
|                            |                          | ``$limit = false``,      | ``isLinkable`` method below.                                              |
|                            |                          | ``$offset = false``,     |                                                                           |
|                            |                          | ``$filter = null``,      |                                                                           |
|                            |                          | ``$ignoreRole = false``  |                                                                           |
+----------------------------+--------------------------+--------------------------+---------------------------------------------------------------------------+
| ``cmf_descendants``        | ``getDescendants``       | ``$parent``,             | Get all repository **paths** of descendants of ``$parent`` (a document    |
|                            |                          | ``$depth = null``        | or a path). ``$depth`` can be used to limit how deep into the hierarchy   |
|                            |                          |                          | you want to descend. If not specified, depth is not limited. No publish   |
|                            |                          |                          | workflow checks are attempted as documents are not even loaded.           |
+----------------------------+--------------------------+--------------------------+---------------------------------------------------------------------------+


Helper methods
..............

+---------------------------+---------------------+------------------------+---------------------------------------------------------------------------+
| ``cmf_document_locales``  | ``getLocalesFor``   | ``$document``,         | Get the locales of the provided document. If ``$includeFallbacks`` is     |
|                           |                     | ``$includeFallbacks``, | ``true``, all fallback locales are provided as well, even if no           |
|                           |                     | ``false``              | translation in that language exists.                                      |
+---------------------------+---------------------+------------------------+---------------------------------------------------------------------------+
| ``cmf_is_linkable``       | ``isLinkable``      | ``$document``          | Check if the provided object can be used to generate a URL. If this       |
|                           |                     |                        | check returns true, it should be save to pass it to ``path`` or ``url``.  |
|                           |                     |                        | An object is considered linkable if it either *is* an instance of         |
|                           |                     |                        | ``Route`` or implements the ``RouteReferrersReadInterface`` *and*         |
|                           |                     |                        | actually returns a route.                                                 |
+---------------------------+---------------------+------------------------+---------------------------------------------------------------------------+
| ``cmf_is_published``      | ``isPublished``     | ``$document``          | Check with the publish workflow if the provided object is published. See  |
|                           |                     |                        | also :ref:`cmf_is_published <bundles-core-publish-workflow-twig_function>`|
|                           |                     |                        | for an example.                                                           |
+---------------------------+---------------------+------------------------+---------------------------------------------------------------------------+

Code examples
.............

.. configuration-block::

    .. code-block:: html+jinja

        {% set page = cmf_find('/some/path') %}

        {% if cmf_is_published(page) %}
            {% set prev = cmf_prev_linkable(page) %}
            {% if prev %}
                <a href="{{ path(prev) }}">prev</a>
            {% endif %}

            {% set next = cmf_next_linkable(page) %}
            {% if next %}
                <span style="float: right; padding-right: 40px;"><a href="{{ path(next) }}">next</a></span>
            {%  endif %}

            {% for news in cmf_children(parent=cmfMainContent, class='AppBundle\\Document\\NewsItem')|reverse %}
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
                            ['_locale' => 'de']
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
                            ['_locale' => 'fr']
                        )
                    )
                ?>">FR</a>
            <?php endif ?>
        <?php endif ?>

.. tip::

    When you use the ``class`` argument, do not forget that Twig will
    simply *ignore* single backslashes. If you would write
    ``AppBundle\Document\NewsItem``, this will make the cmf look
    for the class ``AppBundleDocumentNewsItem`` which will result in an
    empty list. What you need to write in the template is
    ``AppBundle\\Document\\NewsItem``.
