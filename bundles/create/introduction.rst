.. index::
    single: Create; Bundles
    single: CreateBundles

CreateBundle
============

    The CreateBundle provides modern front-end in-place editing for web
    applications. It integrates create.js and the createphp library into
    Symfony2.

The javascript library `create.js`_ provides a comprehensive web editing
interface for Content Management Systems. It is designed to provide a modern,
fully browser-based HTML5 environment for managing content. Create can be
adapted to work on almost any content management backend.
Create.js renders your content editable based on RDFa annotations in the HTML
content. Data is saved through backbone.js, CreateBundle providing the
endpoint for the requests.
The default editor used in the CreateBundle is CKEditor, but you can also use
the lightweight hallo.js editor bundled with the create.js distribution.

`Createphp`_ is a PHP library to help with RDFa annotating your model classes to
make them editable with `create.js`_.

.. index:: CreateBundle

Dependencies
------------

This bundle includes create.js (which bundles all its dependencies like
jquery, vie, hallo, backbone etc) as a git submodule. Do not forget to add the
composer script handler to your ``composer.json`` as described below.

PHP dependencies are managed through composer. We use createphp as well as
AsseticBundle, FOSRestBundle and by inference also JmsSerializerBundle. Make
sure you instantiate all those bundles in your kernel and properly configure
assetic as described below.

To upload and display images the :doc:`MediaBundle <../media/introduction>` is used.

.. _bundle-create-ckeditor:

Installation
------------

You can install this bundle `with composer`_ using the
`symfony-cmf/create-bundle`_ package.

Additionally, you will need to provide the javascript libraries. The standard
way to do this is to add a ``scripts`` section in your ``composer.json`` to
have the CreateBundle download the necessary libraries:

.. code-block:: javascript

    {
        "scripts": {
            "post-install-cmd": [
                "Symfony\\Cmf\\Bundle\\CreateBundle\\Composer\\ScriptHandler::downloadCreateAndCkeditor",
                ...
            ],
            "post-update-cmd": [
                "Symfony\\Cmf\\Bundle\\CreateBundle\\Composer\\ScriptHandler::downloadCreateAndCkeditor",
                ...
            ]
        }
    }

It is possible to specify another directory, repository or commit id in the
extra parameters of ``composer.json`` file. The default values (note that
create commit id will be updated when needed) are:

.. code-block:: javascript

    {
        "extra": {
            "create-directory": "vendor/symfony-cmf/create-bundle/Symfony/Cmf/Bundle/CreateBundle/Resources/public/vendor/create",
            "create-repository": "https://github.com/bergie/create.git",
            "create-commit": "271e0114a039ab256ffcceacdf7f361803995e05"

            "ckeditor-directory": "vendor/symfony-cmf/create-bundle/Symfony/Cmf/Bundle/CreateBundle/Resources/public/vendor/ckeditor",
            "ckeditor-repository": "https://github.com/ckeditor/ckeditor-releases.git",
            "ckeditor-commit": "bba29309f93a1ace1e2e3a3bd086025975abbad0"
        }
    }

Add this bundle (and its dependencies, if they are not already added) to your
application's kernel::

    // app/AppKernel.php
    public function registerBundles()
    {
        return array(
            // ...
            new Symfony\Bundle\AsseticBundle\AsseticBundle(),
            new JMS\SerializerBundle\JMSSerializerBundle($this),
            new FOS\RestBundle\FOSRestBundle(),
            new Symfony\Cmf\Bundle\CreateBundle\CmfCreateBundle(),
            // ...
        );
    }

You also need to configure FOSRestBundle to handle json:

.. configuration-block::

    .. code-block:: yaml

        fos_rest:
            view:
                formats:
                    json: true

    .. code-block:: xml

        <config xmlns="http://example.org/schema/dic/fos_rest">
            <view>
                <format name="json">true</format>
            </view>
        </config>

    .. code-block:: php

        $container->loadFromExtension('fos_rest', array(
            'view' => array(
                'formats' => array(
                    'json' => true,
                ),
            ),
        ));


Routing
~~~~~~~

Finally, you need to register the routing configuration file to your master
routing configuration:

.. configuration-block::

    .. code-block:: yaml

        create:
            resource: "@CmfCreateBundle/Resources/config/routing/rest.xml"

    .. code-block:: xml

        <import resource="@CmfCreateBundle/Resources/config/routing/rest.xml" />

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@CmfCreateBundle/Resources/config/routing/rest.xml"));

        return $collection;

Concepts
--------

Createphp uses `RDFa`_ metadata about your model classes. If you know Doctrine,
you are familiar with this idea, Doctrine has such metadata to know how your
class fields map into database columns. The metadata is modelled by the
``Type`` class. Createphp provides metadata loader that read XML, php arrays
and one that just introspects objects and creates non-semantical metadata that
will be enough for create.js to edit.

An ``RdfMapper`` is used to translate between your storage layer and createphp.
It is passed the model instance and the relevant metadata object.

With the metadata and the twig helper, the content is rendered with RDFa
annotations. create.js is loaded and enables editing on the entities. Save
operations happen in ajax calls to the backend.

The REST controller handles those ajax calls. If you use CKEditor, look into
the :doc:`elfinder documentation<../media/adapters/elfinder>` to enable this
powerful image browser.

Access Control
~~~~~~~~~~~~~~

In order to limit who can edit content, the default REST and image upload
controller as well as the js loader check if the current user is granted the
configured ``role``. By default the role is ROLE_ADMIN, but you can change it
to the role you need.

If you need more fine grained access control, look into the createphp mapper
``isEditable`` method.  You can extend the mapper you use and overwrite
``isEditable`` to answer whether the passed domain object is editable.

Metadata
~~~~~~~~

Createphp needs metadata information for each class of your domain model. By
default, the create bundle uses the XML metadata driver and looks for metadata
in the enabled bundles at ``<Bundle>/Resources/rdf-mappings``. If you use a
third party bundle that does not come with RDFa mapping, you can simply include
a mapping file for it in any bundle, or specify a directory where you put such
mappings with the ``rdf_config_dirs`` option.

The mapping file name needs to be the fully qualified class name of your model
class, having the backslash (``\\``) replaced by a dot (``.``), i.e.
``Symfony.Cmf.Bundle.ContentBundle.Doctrine.Phpcr.StaticContent.xml``.

A basic mapping can look as follows:

.. configuration-block::

    .. code-block:: xml

        <!-- Resources/rdf-mappings/Symfony.Cmf.Bundle.ContentBundle.Doctrine.Phpcr.StaticContent.xml -->
        <type
                xmlns:schema="http://schema.org/"
                typeof="schema:WebPage"
                >
            <children>
                <property property="schema:headline" identifier="title"/>
                <property property="schema:text" identifier="body" />
            </children>
        </type>

The most relevant parts are the ``property`` telling the RDF type, and the
``identifier`` telling the field of the class you map. If you use namespaces
like schema.org, your annotations will actually make semantically sense. But
you can also ignore this and use your own annotations, as long as you do not
use undefined namespaces.

You can additionally specify the HTML tag to be used when automatically
rendering this field (see below). The default tag is ``div``. And you can
specify additional HTML attributes like the ``class`` attribute. A full example
can read like this:

.. configuration-block::

    .. code-block:: xml

        <!-- Resources/rdf-mappings/Symfony.Cmf.Bundle.ContentBundle.Doctrine.Phpcr.StaticContent.xml -->
        <type
                xmlns:schema="http://schema.org/"
                typeof="schema:WebPage"
                >
            <children>
                <property property="schema:headline" identifier="title" tag-name="h1"/>
                <property property="schema:text" identifier="body">
                    <attribute key="class" value="my-css-class"/>
                </property>
            </children>
        </type>

.. note::

    The metadata support in createphp is not as powerful as in Doctrine. There
    are currently no drivers for annotation or yml mappings. Mappings are not
    inherited from a parent class but need to be repeated each time. And the
    mapping file must include the full namespace in the filename to be found.

    All of these issues will hopefully be fixed in later versions.

Mapping Requests to Objects
~~~~~~~~~~~~~~~~~~~~~~~~~~~

At least in version 1.0, the CreateBundle only provides a service to map to
doctrine PHPCR-ODM. If you do not enable the phpcr persistence layer, you
need to configure the ``cmf_create.object_mapper_service_id``.

.. tip::

    Doctrine ORM support is coming soon. There is an open pull request on the
    createphp library to add such a mapper. This mapper will also be provided
    as a service by the CreateBundle 1.1.

Createphp would support specific mappers per RDFa type. If you need that, dig
into the createphp and CreateBundle and do a pull request to enable this feature.

.. _bundle-create-usage-embed:

Rendering RDFa annotated content
--------------------------------

You will need to load the necessary javascript and css files in your templates.

In the page header, include the base CSS files (and add your own CSS files
after those to be able to customize as needed) with

.. code-block:: jinja

    {% include "CmfCreateBundle::includecssfiles.html.twig" %}

.. code-block:: php

    <?php echo $view->render("CmfCreateBundle::includecssfiles.html.twig"); ?>

.. warning::

    Make sure assetic is rewriting the paths in your CSS files properly or you
    might not see icon images.

In your page bottom area load the javascripts. If you are using Symfony 2.2 or
higher, the method reads:

.. configuration-block::

    .. code-block:: jinja

        {% render controller("cmf_create.jsloader.controller:includeJSFilesAction", {'_locale': app.request.locale}) %}

    .. code-block:: php

        <?php $view['actions']->render(
            new
            ControllerReference('cmf_create.jsloader.controller:includeJSFilesAction', array(
                '_locale' => $app->getRequest()->getLocale(),
            ))
        ) ?>

For versions prior to 2.2, this will do:

.. configuration-block::

    .. code-block:: jinja

        {% render "cmf_create.jsloader.controller:includeJSFilesAction" with {'_locale': app.request.locale} %}

    .. code-block:: php

        <?php
        $view['actions']->render('cmf_create.jsloader.controller:includeJSFilesAction', array(
            '_locale' => $app->getRequest()->getLocale(),
        ) ?>

.. note::

    The provided javascript file configures create.js and the editor. For hallo
    a plugin is enabled to use the tag editor to edit ``skos:related``
    collections of attributes. For customization of the editor bootstrap, you
    will need to use a
    :ref:`custom template to load the editor<bundle-create-custom>`.

If you provided RDFa mappings for your model classes as explained above, you
can now adjust your templates to render the RDFa annotations so that create.js
knows what content is editable.

To render your model named ``page`` with a handle you call ``rdf``, use the
``createphp`` twig tag as follows:

.. code-block:: html+jinja

    {% createphp page as="rdf" noautotag %}
    <div {{ createphp_attributes(rdf) }}>
        <h1 class="my-title" {{ createphp_attributes( rdf.title ) }}>{{ createphp_content( rdf.title ) }}</h1>
        <div {{ createphp_attributes( rdf.body ) }}>{{ createphp_content( rdf.body ) }}</div>
    </div>
    {% endcreatephp %}

The ``noautotag`` tells createphp to not automatically output a ``<div>`` with
namespace declarations and the ``about`` property containing the id of your
model. When using ``noautotag``, it is your responsibility to call
``createphp_attributes()`` inside a container tag around all fields.

You can also output a whole field complete with tag, attributes and content by
just calling ``{{ rdf.body|raw }}``. (Without the ``raw`` filter, the HTML
output by createphp would be escaped.) You can even output the whole document
automatically:

.. code-block:: html+jinja

    {% createphp page as="rdf" %}
    {{ rdf|raw }}
    {% endcreatephp %}

This will simply output all fields in the order they appear in the mapping
file. With the optional ``tag-name`` attribute in the mapping file you can
replace the default ``<div>`` tag with your own choice. And using an
``<attribute>`` child to specify CSS classes, you can let createphp generate
your HTML structure if you want.


.. _`Createphp`: https://github.com/flack/createphp
.. _`create.js`: http://createjs.org
.. _`with composer`: http://getcomposer.org
.. _`symfony-cmf/create-bundle`: https://packagist.org/packages/symfony-cmf/create-bundle
.. _`RDFa`: http://en.wikipedia.org/wiki/RDFa