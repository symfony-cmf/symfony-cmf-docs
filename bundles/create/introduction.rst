.. index::
    single: Create; Bundles
    single: CreateBundles

CreateBundle
============

    The CreateBundle provides modern front-end in-place editing for web
    applications. It integrates create.js and the CreatePHP library into
    Symfony2.

The JavaScript library `create.js`_ provides a comprehensive web editing
interface for Content Management Systems. It is designed to provide a modern,
fully browser-based HTML5 environment for managing content. Create.js can be
adapted to work on almost any content management backend. Create.js makes your
content editable based on `RDF`_ information. The CreateBundle provides the
means to load create.js, provide the RDF information and handle the save
requests send by create.js.

For WYSIWYG text, the default editor is `CKEditor`_, but you can also use
the lightweight `hallo.js`_ editor bundled with the create.js distribution
or integrate your own editor.

Concepts
--------

To know the RDF model of your data, create.js parses the page DOM for `RDFa`_
attributes. Whenever it encounters an ``about`` attribute, it knows that this
section is an editable content. The ``typeof`` attribute tells what type the
content has. The ``property`` attributes indicate that the parts inside that tag
are editable. An article might look like this:

.. code-block:: xml

    <div about="/cms/content/home" typeof="schema:WebPage" xmlns:schema="http://schema.org/">
        <h1 property="schema:headline">The Title</h1>
        <div property="schema:text">
            <h2>Welcome to the Symfony CMF Demo</h2>
            <p>If you see this page, it means that the...</p>
        </div>
    </div>

Each property has a type. You can configure what editor to use for which type.
CreateBundle comes with two editors: **plaintext** (with no formatting) and
**WYSIWYG**. You can also define your own editors.

Create.js uses backbone.js to save edited data to the server in the JSON-LD
format. You may have several objects editable on a single page. There will be
one request per editable content that was actually modified.

The `CreatePHP`_ library provides the RDF information for `create.js`_ by
creating `RDFa`_ attributes on your data in the templates. It also maps
the JSON-LD sent by backbone.js back to your domain objects.

The `CreatePHP`_ library contains a metadata tool to define the mapping between
your domain objects and the RDF information. It provides a Twig extension to
enrich your HTML pages with the RDFa attributes, similar to how you output forms.
CreatePHP also provides the means to store the JSON-LD data sent by backbone.js
back into your domain objects and save them. If you know Doctrine, this is a
similar job to how Doctrine reads data from database columns and loads them
into your domain objects.

The CreateBundle finally registers the Twig extension in Symfony and provides
a REST controller for the backbone.js ajax calls. It also provides helpers to
bootstrap create.js in your templates.

Dependencies
------------

The CreateBundle depends on the create.js github repository, which contains all
create.js dependencies like jquery, vie, hallo, backbone and so on. Do not
forget to add the composer script handler to your ``composer.json`` as described
below to have CreateBundle clone that repository for you.

PHP dependencies are managed through composer. Besides the before mentioned
CreatePHP, the CreateBundle also requires the AsseticBundle and the FOSRestBundle
which in turn needs the JmsSerializerBundle. Make sure you load all those
bundles in your kernel and properly configure Assetic as described below.

To upload and display images the :doc:`MediaBundle <../media/introduction>` is
used. CKEditor uses the :doc:`elfinder adapter <../media/adapters/elfinder>`.

.. _bundle-create-ckeditor:

Installation
------------

You can install this bundle `with composer`_ using the
`symfony-cmf/create-bundle`_ package.

Additionally, you will need to provide the JavaScript libraries. The standard
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
should **not hardcode** those in your ``composer.json`` unless you need to
overwrite them) are:

.. code-block:: javascript

    {
        "extra": {
            "create-directory": "vendor/symfony-cmf/create-bundle/Symfony/Cmf/Bundle/CreateBundle/Resources/public/vendor/create",
            "create-repository": "https://github.com/bergie/create.git",
            "create-commit": "a148ce9633535930d7b4b70cc1088102f5c5eb90"

            "ckeditor-directory": "vendor/symfony-cmf/create-bundle/Symfony/Cmf/Bundle/CreateBundle/Resources/public/vendor/ckeditor",
            "ckeditor-repository": "https://github.com/ckeditor/ckeditor-releases.git",
            "ckeditor-commit": "bba29309f93a1ace1e2e3a3bd086025975abbad0"
        }
    }

Add this bundle (and its dependencies, if they are not already added) to your
application's kernel::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Symfony\Bundle\AsseticBundle\AsseticBundle(),
                new JMS\SerializerBundle\JMSSerializerBundle($this),
                new FOS\RestBundle\FOSRestBundle(),
                new Symfony\Cmf\Bundle\CreateBundle\CmfCreateBundle(),
            );

            // ...
        }

        // ...
    }

You also need to configure the FOSRestBundle to handle json:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        fos_rest:
            view:
                formats:
                    json: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/fos_rest">
                <view>
                    <format name="json">true</format>
                </view>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('fos_rest', array(
            'view' => array(
                'formats' => array(
                    'json' => true,
                ),
            ),
        ));

If you want to use Assetic to combine the CSS and JavaScript used for
create.js, you need to enable the CreateBundle in the assetic configuration.
Find the configuration for ``assetic.bundles``. If it is not present, assetic
automatically scans all bundles for assets and you don't need to do anything.
If you limit the bundles, you need to add ``CmfCreateBundle`` to the list of
bundles.

.. configuration-block::

    .. code-block:: yaml

        assetic:
            bundles: [... , CmfCreateBundle]

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://symfony.com/schema/dic/assetic">
                <!-- ... -->
                <bundle>CmfCreateBundle</bundle>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('assetic', array(
            'bundles' => array(
                // ...
                'CmfCreateBundle',
            ),
        ));

Routing
~~~~~~~

You need to register the routing configuration file in your main
routing configuration to enable the REST end point for saving content:

.. configuration-block::

    .. code-block:: yaml

        create:
            resource: "@CmfCreateBundle/Resources/config/routing/rest.xml"

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/dic/routing">

            <import resource="@CmfCreateBundle/Resources/config/routing/rest.xml" />
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@CmfCreateBundle/Resources/config/routing/rest.xml"));

        return $collection;

If you have the :doc:`MediaBundle <../media/index>` present in your project as well, you
additionally need to register the route for the image upload handler:

.. configuration-block::

    .. code-block:: yaml

        create_image:
            resource: "@CmfCreateBundle/Resources/config/routing/image.xml"

    .. code-block:: xml

        <import resource="@CmfCreateBundle/Resources/config/routing/image.xml" />

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@CmfCreateBundle/Resources/config/routing/image.xml"));

        return $collection;

.. _bundle_create_introduction_access_control:

Access Control
~~~~~~~~~~~~~~

In order to limit who can edit content, the provided controllers as well as the
JavaScript loader check if the current user is granted the configured
``cmf_create.security.role``. By default the role is ``ROLE_ADMIN``.

.. tip::

    In order to have security in place, you need to configure a
    "Symfony2 firewall". Read more in the `Symfony2 security chapter`_.
    If you do not do that, create.js will not be loaded and editing
    will be disabled.

    If you do not want to edit on the production domain directly, e.g.
    because of caching, you can provide a second domain where you have
    security configured and do the editing there.

You can completely disable security checks by setting the role parameter to
boolean ``false``. Then you need to configure access permissions on the routes
defined in ``Resources/routing/rest.xml`` and, if activated, in ``image.xml``.
If you set the role to false but do not configure any security,
**every visitor of your site will be able to edit the content**.
You also will need custom logic to decide whether to include the create.js
JavaScript files.

You can also use a custom security check service by implementing
``Symfony\Cmf\Bundle\CreateBundle\Security\AccessCheckerInterface``
and setting this service in ``cmf_create.security.checker_service``.

If you need more fine grained access control, look into the CreatePHP
``RdfMapperInterface`` ``isEditable`` method.  You can extend a mapper and
overwrite ``isEditable`` to answer whether the passed domain object is
editable.

Load create.js JavaScript and CSS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This bundle provides a template that loads the required CSS files, as well as
a controller action that loads the necessary JavaScript *if* the current user
is allowed to edit according to
:ref:`the security configuration <bundle_create_introduction_access_control>`.
The JavaScript loader also parametrizes the configuration for create.js and
WYSIWYG editor.

Alternatively, you can of course use your own templates to include the assets
needed by create.js.

In the page header, include the base CSS files (and add your own CSS files
after those to be able to customize as needed) with:

.. configuration-block::

    .. code-block:: jinja

        {% include "CmfCreateBundle::includecssfiles.html.twig" %}

    .. code-block:: php

        <?php echo $view->render("CmfCreateBundle::includecssfiles.html.twig"); ?>

.. caution::

    Make sure assetic is rewriting the paths in your CSS files properly or you
    might not see icon images.

In your page bottom area, load the JavaScript files. If you are using Symfony 2.2 or
higher, the method reads:

.. configuration-block::

    .. code-block:: jinja

        {% render(controller("cmf_create.jsloader.controller:includeJSFilesAction")) %}

    .. code-block:: php

        <?php $view['actions']->render(
            new ControllerReference('cmf_create.jsloader.controller:includeJSFilesAction')
        ) ?>

.. tip::

    You can include this call unconditionally. The controller checks if the
    current user is allowed to edit and only in that case includes the
    JavaScript.

.. note::

    The provided JavaScript file configures create.js and the editor. If you
    use the hallo editor, a plugin is enabled to use the tag editor to edit
    ``skos:related`` collections of attributes. For customization of the editor
    configuration further, you will need to use a
    :ref:`custom template to load the editor<bundle-create-custom>`.

.. _bundle-create-usage-embed:

Rendering Content
-----------------

Create.js needs to identify what is editable in your content. To do this,
it needs the RDF attributes in the HTML. Now that everything is prepared,
you need to adjust your templates to output that information.

.. note::

    If you use custom types that did not come with RDFa mapping files, see
    the remainder of this page to learn how to define the mappings.

To render your data named ``cmfMainContent`` with a handle you call ``rdf``, use the
``createphp`` Twig tag as follows:

.. code-block:: html+jinja

    {% createphp cmfMainContent as="rdf" noautotag %}
    <div {{ createphp_attributes(rdf) }}>
        <h1 class="my-title" {{ createphp_attributes( rdf.title ) }}>{{ createphp_content( rdf.title ) }}</h1>
        <div {{ createphp_attributes( rdf.body ) }}>{{ createphp_content( rdf.body ) }}</div>
    </div>
    {% endcreatephp %}

The ``noautotag`` tells CreatePHP to not automatically output a ``<div>`` with
namespace declarations and the ``about`` property containing the id of your
object. When using ``noautotag``, it is your responsibility to call
``createphp_attributes()`` inside a container tag that contains the fields of
the object.

You can also output a whole field complete with tag, attributes and content by
just calling ``{{ rdf.body|raw }}``. (Without the ``raw`` filter, the HTML
output by CreatePHP would be escaped.) You can even output the whole document
automatically:

.. code-block:: html+jinja

    {% createphp cmfMainContent as="rdf" %}
    {{ rdf|raw }}
    {% endcreatephp %}

This will simply output all fields in the order they appear in the mapping
file. With the optional ``tag-name`` attribute in the mapping file you can
replace the default ``<div>`` tag with your own choice. And using an
``<attribute>`` child to specify CSS classes, you can let CreatePHP generate
your HTML structure if you want.

Metadata
--------

CreatePHP needs metadata information for each class of your domain objects. By
default, the create bundle uses the XML metadata driver and looks for metadata
in every bundles at ``<Bundle>/Resources/rdf-mappings``. If you use a third
party bundle that does not come with RDFa mapping, you can simply include a
mapping file for it in any of your bundles, or specify a directory containing
mapping files with the ``rdf_config_dirs`` option.

The mapping file name needs to be the fully qualified class name, having the
backslash (``\\``) replaced by a dot (``.``), i.e.
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

.. tip::

    You need to clear the cache when adding a new mapping XML file, even in
    the dev environment. The CreateBundle caches where it found mapping files
    to avoid scanning all folders on every request. Once a file is known, edits
    will be picked automatically, without the need to clear the cache again.

You can additionally specify the HTML tag to be used when automatically
rendering this field (see below). The default tag is ``div``. And you can
specify additional HTML attributes like the ``class`` attribute. A full example
reads like this:

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

Mapping Requests to Domain Objects
----------------------------------

One last piece is the mapping between CreatePHP data and the application
domain objects. Data needs to be stored back into the database.

In version 1.0, the CreateBundle only provides a service to map to Doctrine
PHPCR-ODM. If you do not enable the phpcr persistence layer, you need to
configure the ``cmf_create.object_mapper_service_id``.

.. tip::

    Doctrine ORM support is coming soon. There is an open pull request on the
    CreatePHP library to add such a mapper. This mapper will also be provided
    as a service by the CreateBundle 1.1.

CreatePHP would support specific mappers per RDFa type. If you need that, dig
into the CreatePHP and CreateBundle and do a pull request to enable this feature.


.. _`create.js`: http://createjs.org
.. _`hallo.js`: http://hallojs.org
.. _`CKEditor`: http://ckeditor.com/
.. _`CreatePHP`: https://github.com/flack/createphp
.. _`with composer`: http://getcomposer.org
.. _`symfony-cmf/create-bundle`: https://packagist.org/packages/symfony-cmf/create-bundle
.. _`RDF`: http://en.wikipedia.org/wiki/Resource_Description_Framework
.. _`RDFa`: http://en.wikipedia.org/wiki/RDFa
.. _`Symfony2 security chapter`: http://symfony.com/doc/current/book/security.html
