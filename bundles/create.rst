.. index::
    single: Create; Bundles
    single: CreateBundles

CreateBundle
============

The `CreateBundle`_ integrates create.js and the createphp helper library into
Symfony2.

Create.js is a comprehensive web editing interface for Content Management
Systems. It is designed to provide a modern, fully browser-based HTML5
environment for managing content. Create can be adapted to work on almost any
content management backend. The default editor is the Hallo Editor, but you
can also use CKEditor. See http://createjs.org/ for more information.

Createphp is a PHP library to help with RDFa annotating your
documents/entities. See https://github.com/flack/createphp for documentation
on how it works.

.. index:: CreateBundle

Dependencies
------------

This bundle includes create.js (which bundles all its dependencies like
jquery, vie, hallo, backbone etc) as a git submodule. Do not forget to add the
composer script handler to your ``composer.json`` as described below.

PHP dependencies are managed through composer. We use createphp as well as
AsseticBundle, FOSRestBundle and by inference also JmsSerializerBundle. Make
sure you instantiate all those bundles in your kernel and properly configure
assetic.

Installation
------------

This bundle is best included using Composer.

Edit your project composer file to add a new require for
``symfony-cmf/create-bundle``. Then create a scripts section or add to the
existing one:

.. code-block:: javascript

    {
        "scripts": {
            "post-install-cmd": [
                "Symfony\\Cmf\\Bundle\\CreateBundle\\Composer\\ScriptHandler::downloadCreate",
                ...
            ],
            "post-update-cmd": [
                "Symfony\\Cmf\\Bundle\\CreateBundle\\Composer\\ScriptHandler::downloadCreate",
                ...
            ]
        }
    }

It is possible to specify another directory, repository or commit id in the
extra parameters of ``composer.json`` file (here are the default values):

.. code-block:: javascript

    {
        "extra": {
            "create-directory": "vendor/symfony-cmf/create-bundle/Symfony/Cmf/Bundle/CreateBundle/Resources/public/vendor/create",
            "create-repository": "https://github.com/bergie/create.git",
            "create-commit": "271e0114a039ab256ffcceacdf7f361803995e05"
        }
    }

Add this bundle (and its dependencies, if they are not already there) to your
application's kernel::

    // application/ApplicationKernel.php
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

.. _bundle-create-ckeditor:

Using CKEditor Instead
~~~~~~~~~~~~~~~~~~~~~~

If you want to use CKEditor, edit the ``composer.json`` file to call
``downloadCreateAndCkeditor`` instead of ``downloadCreate``:

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

and re-run composer:

.. code-block:: bash

    $ php composer.phar update nothing

If you use a non-standard setup, you can adjust the editor base path
configuration. The default value is:

.. configuration-block::

    .. code-block:: yaml

        cmf_create:
            editor_base_path: /bundles/cmfcreate/vendor/ckeditor/

    .. code-block:: xml

        <cmf-create:config
            editor-base-path="/bundles/cmfcreate/vendor/ckeditor/"
        />

    .. code-block:: php

        $container->loadFromExtension('cmf_create', array(
            'editor_base_path': '/bundles/cmfcreate/vendor/ckeditor/',
        ));

In your template, load the javascript files using:

.. configuration-block::

    .. code-block:: jinja

        {% render controller(
            "cmf_create.jsloader.controller:includeJSFilesAction",
            {"editor": "ckeditor"}
        ) %}

    .. code-block:: php

        <?php $view['actions']->render(
            'cmf_create.jsloader.controller:includeJSFilesAction',
            array(
                'editor' => 'ckeditor',
            )
        ) ?>

As for create.js, you can override the source of CKEditor to a different
target directory, source repository or commit id in the extra parameters of
the ``composer.json`` file (here are the default values):

.. code-block:: javascript

    {
        "extra": {
            "ckeditor-directory": "vendor/symfony-cmf/create-bundle/Symfony/Cmf/Bundle/CreateBundle/Resources/public/vendor/ckeditor",
            "ckeditor-repository": "https://github.com/ckeditor/ckeditor-releases.git",
            "ckeditor-commit": "bba29309f93a1ace1e2e3a3bd086025975abbad0"
        }
    }

Concept
-------

Createphp uses RDFa metadata about your domain classes, much like doctrine
knows the metadata how an object is stored in the database. The metadata is
modelled by the type class and can come from any source. Createphp provides
metadata drivers that read XML, php arrays and one that just introspects
objects and creates non-semantical metadata that will be enough for create.js
to edit.

The RdfMapper is used to translate between your storage layer and createphp.
It is passed the domain object and the relevant metadata object.

With the metadata and the twig helper, the content is rendered with RDFa
annotations. create.js is loaded and enables editing on the entities. Save
operations happen in ajax calls to the backend.

The REST controller handles those ajax calls, and if you want to be able to
upload images, an image controller saves uploaded images and tells the image
location.

Configuration
-------------

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_create:
            # metadata loading

            # directory list to look for metadata
            rdf_config_dirs:
                - "%kernel.root_dir%/Resources/rdf-mappings"
            # look for mappings in <Bundle>/Resources/rdf-mappings
            # auto_mapping: true

            # use a different class for the REST handler
            # rest_controller_class: FQN\Classname

            # image handling
            image:
                model_class: ~
                controller_class: ~

            # access check role for js inclusion, default REST and image controllers
            # role: IS_AUTHENTICATED_ANONYMOUSLY

            # enable the doctrine PHPCR-ODM mapper
            phpcr_odm: true

            # mapping from rdf type name => class name used when adding items to collections
            map:
                rdfname: FQN\Classname

            # stanbol url for semantic enhancement, otherwise defaults to the demo install
            # stanbol_url: http://dev.iks-project.eu:8081

            # fix the Hallo editor toolbar on top of the page
            # fixed_toolbar: true

            # RDFa types used for elements to be edited in plain text
            # plain_text_types: ['dcterms:title']

            # RDFa types for which to create the corresponding routes after
            # content of these types has been added with Create.js. This is
            # not necessary with the SimpleCmsBundle, as the content and the
            # routes are in the same repository tree.
            # create_routes_types: ['http://schema.org/NewsArticle']

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <!--
            auto-mapping: look for mappings in <Bundle>/Resources/rdf-mappings
            rest-controller-class: use a different class for the REST handler
            role: access check role for js inclusion, default REST and image controllers
            phpcr-odm: enable the doctrine PHPCR-ODM mapper
            stanbol-url: stanbol url for semantic enhancement, otherwise defaults to the demo install
            fixed-toolbar: fix the Hallo editor toolbar on top of the page
        -->
        <config xmlns="http://cmf.symfony.com/schema/dic/create"
            auto-mapping="true"
            rest-controller-class="FQN\ClassName"
            role="IS_AUTHENTICATED_ANONYMOUSLY"
            phpcr-odm="true"
            stanbol-url="http://dev.iks-project.eu:8081"
            fixed-toolbar="true">
            <!-- metadata loading -->

            <!-- directory list to look for metadata -->
            <rdf-config-dir>%kernel.root_dir%/Resources/rdf-mappings</rdf-config-dir>

            <!-- image handling -->
            <image
                model-class=""
                controller-class=""
            />

            <!-- mapping from rdf type name => class name used when adding items to collections -->
            <map
                rdfname="FQN\ClassName"
            />

            <!-- RDFa types used for elements to be edited in plain text -->
            <plain-text-type>dcterms:title</plain-text-type>
            <!--
                RDFa types for which to create the corresponding routes after
                content of these types has been added with Create.js. This is
                not necessary with the SimpleCmsBundle, as the content and the
            -->
        </config>

    .. code-block:: php

        // app/config/config.yml
        $container->loadFromExtension('cmf_create', array(
            // metadata loading

            // directory list to look for metadata
            'rdf_config_dirs' => array(
                "%kernel.root_dir%/Resources/rdf-mappings",
            ),

            // look for mappings in <Bundle>/Resources/rdf-mappings
            // 'auto_mapping' => true,

            // use a different class for the REST handler
            // 'rest_controller_class' => 'FQN\Classname'

            // image handling
            'image' => array(
                'model_class'      => null,
                'controller_class' => null,
            ),

            // access check role for js inclusion, default REST and image controllers
            // 'role' => 'IS_AUTHENTICATED_ANONYMOUSLY',

            // enable the doctrine PHPCR-ODM mapper
            'phpcr_odm' => true,

            // mapping from rdf type name => class name used when adding items to collections
            'map' => array(
                'rdfname' => 'FQN\Classname',
            ),

            // stanbol url for semantic enhancement, otherwise defaults to the demo install
            // 'stanbol_url' => 'http://dev.iks-project.eu:8081',

            // fix the Hallo editor toolbar on top of the page
            // 'fixed_toolbar' => true,

            // RDFa types used for elements to be edited in plain text
            // 'plain_text_types' => array('dcterms:title'),

            // RDFa types for which to create the corresponding routes after
            // content of these types has been added with Create.js. This is
            // not necessary with the SimpleCmsBundle, as the content and the
        ));

The provided javascript file configures create.js and the hallo editor. It
enables some plugins like the tag editor to edit ``skos:related`` collections
of attributes. We hope to add some configuration options to tweak the
configuration of create.js but you can also use the file as a template and do
your own if you need larger customizations.

Metadata
~~~~~~~~

Createphp needs metadata information for each class of your domain model. By
default, the create bundle uses the XML metadata driver and looks for metadata
in the enabled bundles at ``<Bundle>/Resources/rdf-mappings``. If you use a
bundle that has no RDFa mapping, you can specify a list of ``rdf_config_dirs``
that will additionally be checked for metadata.

See the `documentation of createphp`_ for the format of the XML metadata format.

Access Control
~~~~~~~~~~~~~~

If you use the default REST controller, everybody can edit content once you
enabled the create bundle. To restrict access, specify a role other than the
default IS_AUTHENTICATED_ANONYMOUSLY to the bundle. If you specify a
different role, create.js will only be loaded if the user has that role and
the REST handler (and image handler if enabled) will check the role.

If you need more fine grained access control, look into the mapper
``isEditable`` method.  You can extend the mapper you use and overwrite
isEditable to answer whether the passed domain object is editable.

Image Handling
~~~~~~~~~~~~~~

Enable the default simplistic image handler with the image > model_class |
controller_class settings. This image handler just throws images into the
PHPCR-ODM repository and also serves them in requests.

If you need different image handling, you can either overwrite
``image.model_class`` and/or ``image.controller_class``, or implement a custom
``ImageController`` and override the ``cmf_create.image.controller``
service with it.

Mapping Requests to Objects
~~~~~~~~~~~~~~~~~~~~~~~~~~~

For now, the bundle only provides a service to map to doctrine PHPCR-ODM.
Enable it by setting ``phpcr_odm`` to true. If you need something else, you need
to provide a service ``cmf_create.object_mapper``. (If you need a
wrapper for doctrine ORM, look at the mappers in the createphp library and do
a pull request on that library, and another one to expose the ORM mapper as
service in the create bundle).

Also note that createphp would support different mappers for different RDFa
types.  If you need that, dig into the createphp and create bundle and do a
pull request to enable this feature.

To be able to create new objects, you need to provide a map between the RDFa
types and the class names.

.. TODO: can we not index all mappings and do this automatically?

Routing
~~~~~~~

Finally add the relevant routing to your configuration

.. configuration-block::

    .. code-block:: yaml

        create:
            resource: "@CmfCreateBundle/Resources/config/routing/rest.xml"
        create_image:
            resource: "@CmfCreateBundle/Resources/config/routing/image.xml"

    .. code-block:: xml

        <import resource="@CmfCreateBundle/Resources/config/routing/rest.xml" />
        <import resource="@CmfCreateBundle/Resources/config/routing/image.xml" />

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@CmfCreateBundle/Resources/config/routing/rest.xml"));
        $collection->addCollection($loader->import("@CmfCreateBundle/Resources/config/routing/image.xml"));

        return $collection;

.. _bundle-create-usage-embed:

Usage
-----

Adjust your template to load the editor js files if the current session is
allowed to edit content.

If you are using Symfony 2.2 or higher:

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

Plus make sure that assetic is rewriting paths in your css files, then
include the base css files (and customize with your css as needed) with

.. code-block:: jinja

    {% include "CmfCreateBundle::includecssfiles.html.twig" %}

The other thing you have to do is provide RDFa mappings for your model classes
and adjust your templates to render with createphp so that create.js knows
what content is editable.

Create XML metadata mappings in ``<Bundle>/Resources/rdf-mappings`` or a path
you configured in rdf_config_dirs named after the full classname of your model
classes with ``\\`` replaced by a dot (``.``), i.e.
``Symfony.Cmf.Bundle.SimpleCmsBundle.Document.MultilangPage.xml``. For an
example mapping see the files in the cmf-sandbox. Reference documentation is
in the `createphp library repository`_.

To render your model, use the createphp twig tag:

.. code-block:: html+jinja

    {% createphp page as="rdf" %}
    {{ rdf|raw }}
    {% endcreatephp %}

Or if you need more control over the generated HTML:

.. code-block:: html+jinja

    {% createphp page as="rdf" %}
    <div {{ createphp_attributes(rdf) }}>
        <h1 class="my-title" {{ createphp_attributes( rdf.title ) }}>{{ createphp_content( rdf.title ) }}</h1>
        <div {{ createphp_attributes( rdf.body ) }}>{{ createphp_content( rdf.body ) }}</div>
    </div>
    {% endcreatephp %}

Alternative Editors
~~~~~~~~~~~~~~~~~~~

You can write your own templates to load a javascript editor. They have to
follow the naming pattern
``CmfCreateBundle::includejsfiles-%editor%.html.twig`` to be loaded. In
the includeJSFilesAction, you specify the editor parameter.  (Do not forget to
add the ``controller`` call around the controller name inside ``render`` for
Symfony 2.2, as in the example above.)

.. configuration-block::

    .. code-block:: jinja

        {% render "cmf_create.jsloader.controller:includeJSFilesAction" with {'editor': 'aloha', '_locale': app.request.locale } %}

    .. code-block:: php

        <?php
        $view['actions']->render('cmf_create.jsloader.controller:includeJSFilesAction', array(
            'editor'  => 'aloha',
            '_locale' => $app->getRequest()->getLocale(),
        ));

.. note::

    Create.js has built in support for Aloha and ckeditor, as well as the
    default hallo editor. Those should be supported by the CreateBundle as
    well. See these github issue for `ckeditor`_ and `aloha`_ integration.

    If you wrote the necessary code for one of those editors, or another
    editor that could be useful for others, please send a pull request.

Developing the Hallo Wysiwyg Editor
-----------------------------------

You can develop the hallo editor inside the Create bundle. By default, a
minimized version of hallo that is bundled with create is used. To develop the
actual code, you will need to checkout the full hallo repository first. You
can do this by running the following command from the command line:

.. code-block:: bash

    $ php app/console cmf:create:init-hallo-devel

There is a special template to load the coffee script files. To load this,
just use the ``hallo-coffee`` editor with the includeJSFilesAction.  (Do not
forget to add the ``controller`` call around the controller name inside
``render`` for Symfony 2.2, as in the example above.)

.. configuration-block::

    .. code-block:: jinja

        {% render "cmf_create.jsloader.controller:includeJSFilesAction" with {'editor': 'hallo-coffee', '_locale': app.request.locale } %}

    .. code-block:: php

        <?php
        $view['actions']->render('cmf_create.jsloader.controller:includeJSFilesAction", array(
            'editor'  => 'hallo-coffee',
            '_locale' => $app->getRequest()->getLocale(),
        )) ?>

The hallo-coffee template uses assetic to load the coffee script files from
``Resources/public/vendor/hallo/src``, rather than the precompiled javascript
from ``Resources/public/vendor/create/deps/hallo-min.js``. This also means
that you need to add a mapping for coffeescript in your assetic configuration
and you need the `coffee compiler set up correctly`_.

.. configuration-block::

    .. code-block:: yaml

        assetic:
            filters:
                cssrewrite: ~
                coffee:
                    bin: %coffee.bin%
                    node: %coffee.node%
                    apply_to: %coffee.extension%

    .. code-block:: xml

        <config xmlns="http://symfony.com/schema/dic/assetic">
            <filter name="cssrewite" />
            <filter name="coffee"
                bin="%coffee.bin%"
                node="%coffee.node%"
                apply-to="%coffee.extension%" />
        </config>

    .. code-block:: php

        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'cssrewrite' => null,
                'coffee'     => array(
                    'bin'      => '%coffee.bin%',
                    'node'     => '%coffee.node%',
                    'apply_to' => '%coffee.extension%',
                ),
            ),
        ));

In the cmf sandbox we did a little hack to not alwas trigger coffee script
compiling.  In config.yml we make the coffee extension configurable. Now if
the parameters.yml sets ``coffee.extension`` to ``\.coffee`` the coffeescript
is compiled and the coffee compiler needs to be installed. If you set it to
anything else like ``\.nocoffee`` then you do not need the coffee compiler
installed.

The default values for the three parameters are

.. code-block:: yaml

    # app/config/parameters.yml

    # ...
    coffee.bin: /usr/local/bin/coffee
    coffee.node: /usr/local/bin/node
    coffee.extension: \.coffee

.. _`CreateBundle`: https://github.com/symfony-cmf/CreateBundle
.. _`documentation of createphp`: https://github.com/flack/createphp
.. _`createphp library repository`: https://github.com/flack/createphp
.. _`ckeditor`: https://github.com/symfony-cmf/CreateBundle/issues/33
.. _`aloha`: https://github.com/symfony-cmf/CreateBundle/issues/32
.. _`coffee compiler set up correctly`: http://coffeescript.org/#installation
