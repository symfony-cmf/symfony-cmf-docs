.. index::
    single: Create; Bundles
    single: CreateBundles

CreateBundle
============

    The CreateBundle provides modern front-end in-place editing for web
    applications. It integrates create.js and the CreatePHP library into
    Symfony2.

The javascript library `create.js`_ provides a comprehensive web editing
interface for Content Management Systems. It is designed to provide a modern,
fully browser-based HTML5 environment for managing content. Create.js can be
adapted to work on almost any content management backend. Create.js makes your
content editable based on RDFa annotations in the HTML content. Data is saved
through backbone.js, CreateBundle providing the endpoint for the requests.
The default editor used in the CreateBundle is CKEditor, but you can also use
the lightweight `hallo.js`_ editor bundled with the create.js distribution.

`CreatePHP`_ is a PHP library that helps you putting the RDFa attributes into
your HTML for `create.js`_ and map back from the JSON-LD sent by backbone.js
to your model classes.

.. index:: CreateBundle

Dependencies
------------

The CreateBundle depends on the create.js github repository, which contains all
create.js dependencies like jquery, vie, hallo, backbone and so on. Do not
forget to add the composer script handler to your ``composer.json`` as described
below to have CreateBundle clone that repository for you.

PHP dependencies are managed through composer. Besides the aforementioned
CreatePHP, CreateBundle also needs the AsseticBundle and the FOSRestBundle
which in turn needs the JmsSerializerBundle. Make sure you load all those
bundles in your kernel and properly configure Assetic as described below.

To upload and display images the :doc:`MediaBundle <../media/introduction>` is
used.

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

It is possible to specify another target directory, repository URL or commit
id in the extra parameters of ``composer.json`` file if you need to use a
development version of CKEditor or create.js. The default values (note that you
should **not hardcode** those in your composer.json unless you need to
overwrite them) are:

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

You also need to configure the FOSRestBundle to handle json:

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
routing configuration to enable the REST end point for saving content:

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

Access Control
~~~~~~~~~~~~~~

In order to limit who can edit content, the provided controllers as well as the
javascript loader check if the current user is granted the configured
``cmf_create.role``. By default the role is ROLE_ADMIN.

If you need more fine grained access control, look into the CreatePHP
``RdfMapperInterface`` ``isEditable`` method.  You can extend a mapper and
overwrite ``isEditable`` to answer whether the passed domain object is
editable.

Concepts
--------

CreatePHP uses `RDFa`_ metadata about your model classes. If you know Doctrine,
you should be familiar with this concept, as Doctrine uses such metadata to
know how your class fields map to database columns.

The metadata is modelled by the ``Type`` class. CreatePHP provides metadata
loaders that read XML, php arrays and one that just introspects objects and
creates non-semantical metadata that will be enough for create.js to edit.

An ``RdfMapper`` is used to translate between your storage layer and CreatePHP.
It is passed the model instance and the relevant metadata object.

With the metadata and the twig helper, the content is rendered with RDFa
annotations. create.js is loaded and enables editing the content. Note that
you may have several objects editable on a single page. Save operations happen
through backbone.js with ajax calls containing JSON-LD data. There is one
request per editable content that was actually modified. The CreateBundle REST
controller handles those ajax calls and maps the JSON-LD data back onto your
model classes and stores them in the database.

For image support, CKEditor can use elfinder to upload, browse and insert
images into the content. See the
:doc:`MediaBundle elfinder adapter documentation<../media/adapters/elfinder>`
to enable this powerful image browser.

Metadata
~~~~~~~~

CreatePHP needs metadata information for each class of your domain model. By
default, the create bundle uses the XML metadata driver and looks for metadata
in every bundles at ``<Bundle>/Resources/rdf-mappings``. If you use a third
party bundle that does not come with RDFa mapping, you can simply include a
mapping file for it in any of your bundles, or specify a directory containing
mapping files with the ``rdf_config_dirs`` option.

The mapping file name needs to be the fully qualified class name of your model
class, having the backslash (``\\``) replaced by a dot (``.``), i.e.
``Symfony.Cmf.Bundle.ContentBundle.Doctrine.Phpcr.StaticContent.xml``.

A basic mapping look as follows:

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
you can also ignore this and use your own annotations, as long as you declare
the namespaces you use.

.. hint::

    You need to clear the cache when adding a new mapping XML file, even in
    the dev environment. The CreateBundle caches where it found mapping files
    to avoid scanning all folders on every request. Once a file is known, edits
    will be picked automatically, without the need to clear the cache again.

You can additionally specify the HTML tag to be used when automatically
rendering this field (see below). The default tag is ``div``. And you can
specify additional HTML attributes like the ``class`` attribute. A full example
reads like this:

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

    The metadata support in CreatePHP is not as powerful as in Doctrine. There
    are currently no drivers for annotation or yml mappings. Mappings are not
    inherited from a parent class but need to be repeated each time. And the
    mapping file must include the full namespace in the filename to be found.

    All of these issues will hopefully be fixed in later versions if people
    step up and contribute pull requests.

Mapping Requests to Objects
~~~~~~~~~~~~~~~~~~~~~~~~~~~

In version 1.0, the CreateBundle only provides a service to map to Doctrine
PHPCR-ODM. If you do not enable the phpcr persistence layer, you need to
configure the ``cmf_create.object_mapper_service_id``.

.. tip::

    Doctrine ORM support is coming soon. There is an open pull request on the
    CreatePHP library to add such a mapper. This mapper will also be provided
    as a service by the CreateBundle 1.1.

CreatePHP would support specific mappers per RDFa type. If you need that, dig
into the CreatePHP and CreateBundle and do a pull request to enable this feature.

.. _bundle-create-usage-embed:

Rendering Content
-----------------

Rendering the content for create.js consists of adjusting how you output your
model classes and of loading the necessary javascript and css files.

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

        {% render(controller(
            "cmf_create.jsloader.controller:includeJSFilesAction",
            {'_locale': app.request.locale}
        )) %}

    .. code-block:: php

        <?php $view['actions']->render(
            new ControllerReference('cmf_create.jsloader.controller:includeJSFilesAction', array(
                '_locale' => $app->getRequest()->getLocale(),
            ))
        ) ?>

For Symfony 2.1, the syntax is:

.. configuration-block::

    .. code-block:: jinja

        {% render "cmf_create.jsloader.controller:includeJSFilesAction" with {'_locale': app.request.locale} %}

    .. code-block:: php

        <?php
        $view['actions']->render('cmf_create.jsloader.controller:includeJSFilesAction', array(
            '_locale' => $app->getRequest()->getLocale(),
        ) ?>

.. note::

    The provided javascript file configures create.js and the editor. If you
    use the hallo editor, a plugin is enabled to use the tag editor to edit
    ``skos:related`` collections of attributes. For customization of the editor
    configuration further, you will need to use a
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

The ``noautotag`` tells CreatePHP to not automatically output a ``<div>`` with
namespace declarations and the ``about`` property containing the id of your
model. When using ``noautotag``, it is your responsibility to call
``createphp_attributes()`` inside a container tag that contains all fields of
one model instance.

You can also output a whole field complete with tag, attributes and content by
just calling ``{{ rdf.body|raw }}``. (Without the ``raw`` filter, the HTML
output by CreatePHP would be escaped.) You can even output the whole document
automatically:

.. code-block:: html+jinja

    {% createphp page as="rdf" %}
    {{ rdf|raw }}
    {% endcreatephp %}

This will simply output all fields in the order they appear in the mapping
file. With the optional ``tag-name`` attribute in the mapping file you can
replace the default ``<div>`` tag with your own choice. And using an
``<attribute>`` child to specify CSS classes, you can let CreatePHP generate
your HTML structure if you want.


.. _`create.js`: http://createjs.org
.. _`hallo.js`: http://hallojs.org
.. _`CreatePHP`: https://github.com/flack/createphp
.. _`with composer`: http://getcomposer.org
.. _`symfony-cmf/create-bundle`: https://packagist.org/packages/symfony-cmf/create-bundle
.. _`RDFa`: http://en.wikipedia.org/wiki/RDFa
