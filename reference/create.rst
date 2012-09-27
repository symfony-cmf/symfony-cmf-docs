SymfonyCmfCreateBundle
======================

The `SymfonyCmfCreateBundle <https://github.com/symfony-cmf/CreateBundle#readme>`_
integrates create.js and the createphp helper into Symfony2.

Create.js is a comprehensive web editing interface for Content Management
Systems. It is designed to provide a modern, fully browser-based HTML5
environment for managing content. Create can be adapted to work on almost any
content management backend.
See http://createjs.org/

Createphp is a php library to help with RDFa annotating your documents/entities.
See https://github.com/flack/createphp for documentation on how it works.

.. index:: CreateBundle

Dependencies
------------

This bundle includes create.js (which bundles all its dependencies like jquery,
vie, hallo, backbone etc) as a git submodule. Do not forget to add the composer
script handler to your composer.json as described above.

PHP dependencies are managed through composer. We use createphp as well as
AsseticBundle, FOSRestBundle and by inference also JmsSerializerBundle. Make
sure you instantiate all those bundles in your kernel and properly configure
assetic.

Installation
------------

This bundle is best included using Composer.

Edit your project composer file to add a new require for symfony-cmf/create-bundle.
Then create a scripts section or add to the existing one::

    "scripts": {
        "post-install-cmd": [
            "Symfony\\Cmf\\Bundle\\CreateBundle\\Composer\\ScriptHandler::initSubmodules",
            ...
        ],
        "post-update-cmd": [
            "Symfony\\Cmf\\Bundle\\CreateBundle\\Composer\\ScriptHandler::initSubmodules",
            ...
        ]
    },

Add this bundle (and its dependencies, if they are not already there) to your
application's kernel::

    // application/ApplicationKernel.php
    public function registerBundles()
    {
        return array(
            // ...
            new JMS\SerializerBundle\JMSSerializerBundle($this),
            new FOS\RestBundle\FOSRestBundle(),
            new Symfony\Cmf\Bundle\CreateBundle\SymfonyCmfCreateBundle(),
            // ...
        );
    }


Configuration
-------------

Add a mapping to the ``config.yml`` and enable a controller::

        symfony_cmf_create:
            phpcr_odm: true
            #orm: my_document_manager
            rdf_config_dirs:
                - app/Resources/rdf-mappings
            base_path: /cms/routes
            cms_path: /cms/content/static
            # stanbol_url: custom stanbol url, otherwise defaults to the demo install

You need to implement an ImageController and override the ``symfony_cmf_create.image.controller``
service accordingly.

Finally add the relevant routing to your configuration

        <import resource="symfony_cmf_create.rest.controller" type="rest" />

        createjs:
            resource: "@SymfonyCmfCreateBundle/Resources/config/routing/rest.xml"

Optional: Aloha Editor (this bundle comes with the hallo editor, but if you prefer you can also use aloha)

        To use the Aloha editor with this bundle, download the files here: https://github.com/alohaeditor/Aloha-Editor/downloads/aloha-0.9.3.zip
        Unzip the contents of the "aloha" subfolder of this zip as folder vendor/symfony-cmf/create-bundle/Symfony/Cmf/Bundlle/CreateBundle/vendor/aloha
        Make sure you have just one aloha folder with the js, not aloha/aloha/... - you should have vendor/symfony-cmf/create-bundle/Symfony/Cmf/Bundlle/CreateBundle/vendor/aloha/aloha.js


Usage
-----

Adjust your template to load the editor js files if the current session is allowed to edit content.

::

    {% render "symfony_cmf_create.jsloader.controller:includeJSFilesAction" %}

Plus make sure that assetic is rewriting paths in your css files, then  include
the base css files (and customize with your css as needed) with

::

    {% include "SymfonyCmfCreateBundle::includecssfiles.html.twig" %}

The other thing you have to do is provide RDFa mappings for your model classes
and adjust your templates to render with createphp so that create.js knows what
content is editable.

Create xml files in the paths you configured in rdf_config_dirs named after the
full classname of your model classes with ``\\`` replaced by ``.``. For an
example mapping see the files in the cmf-sandbox. Reference documentation is in
the [createphp library repository](https://github.com/flack/createphp).

To render your model, use the createphp twig tag::

    {% createphp page as="rdf" %}
    {{ rdf|raw }}
    {% endcreatephp %}

Or if you need more control over the generated HTML::

    {% createphp page as="rdf" %}
    <div {{ createphp_attributes(rdf) }}>
        <h1 class="my-title" {{ createphp_attributes( rdf.title ) }}>{{ createphp_content( rdf.title ) }}</h1>
        <div {{ createphp_attributes( rdf.body ) }}>{{ createphp_content( rdf.body ) }}</div>
    </div>
    {% endcreatephp %}

Developing the hallo wysiwyg editor
-----------------------------------

You can develop the hallo editor inside the Create bundle. By default, a minimized
version of hallo that is bundled with create is used. To develop the actual code,
you will need to checkout the full hallo repository first. You can do this by running
the following commenad from the command line:

    app/console cmf:create:init-hallo-devel

Then, set the ``use_coffee`` option to true in config.yml. This tells the
jsloader to include the coffee script files from
``Resources/public/vendor/hallo/src`` with assetic, rather than the precompiled
javascript from ``Resources/public/vendor/create/deps/hallo-min.js``.
This also means that you need to add a mapping for coffeescript in your assetic
configuration and you need the [coffee compiler set up correctly](http://coffeescript.org/#installation).

::

    assetic:
        filters:
            cssrewrite: ~
            coffee:
                bin: %coffee.bin%
                node: %coffee.node%
                apply_to: %coffee.extension%

    symfony_cmf_create:
        # set this to true if you want to develop hallo and edit the coffee files
        use_coffee: true|false

In the cmf sandbox we did a little hack to not trigger coffee script compiling.
In config.yml we make the coffee extension configurable. Now if the
parameters.yml sets ``coffee.extension`` to ``\.coffee`` the coffeescript is
compiled and the coffee compiler needs to be installed. If you set it to
anything else like ``\.nocoffee`` then you do not need the coffee compiler
installed.

The default values for the three parameters are::

    coffee.bin: /usr/local/bin/coffee
    coffee.node: /usr/local/bin/node
    coffee.extension: \.coffee