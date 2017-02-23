.. index::
    single: Tree; TreeBrowserBundle

Displaying Nodes inside the Tree
================================

The TreeBrowserBundle provides a ``cmfTree`` jQuery plugin. This plugin is an
abstraction on top of Fancytree_. It also allows to integrate other JavaScript
tree implementations.

The plugin is based on the API provided by the CmfResourceRestBundle.

Usage
-----

The plugin requires one option: ``request.load``. This option should contain a
callable that returns `an AJAX settings object`_ as supported by Fancytree:

.. code-block:: twig+html

    {# the Tree will be displayed here #}
    <div id="tree-output"></div>

    {# include required CSS and JS files #}
    {{ include('@CmfTreeBrowser/Base/tree.html.twig') }}

    <script>
    $('#tree-output').cmfTree({
        request: {
            load: function (nodePath) {
                // nodePath is the PHPCR path of the node to load
                return {
                    url: '/api/default' + nodePath
                };
            }
        }
    });
    </script>

Using this code, a tree will be shown in the ``<div>`` tag. The
``/api/default/`` URI will be used to request for nodes. Note that this
happends lazily, so after opening a node with no children loaded yet, the URL
will again be requested for the children (e.g. ``/api/default/cms/content``
when opening the ``/cms/content`` node).

Options
-------

* adapter
* request
  * load
  * move
* actions
* path_output

(fancytree)
* root_node
* use_cache
* dnd
  * enabled
  * isNodeDraggable
  * nodeAcceptsDraggable

.. _Fancytree: https://github.com/mar10/fancytree
.. _an AJAX settings object: https://github.com/mar10/fancytree/wiki/TutorialLoadData#load-the-data-via-ajax
