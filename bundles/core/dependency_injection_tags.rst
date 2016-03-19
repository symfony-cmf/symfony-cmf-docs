.. index::
    single: Dependency Injection Tags; CoreBundle

Dependency Injection Tags
-------------------------

cmf_published_voter
~~~~~~~~~~~~~~~~~~~

Used to activate :ref:`custom voters <bundle-core-workflow-custom-voters>` for the
:doc:`publish workflow <publish_workflow>`. Tagging a service with
``cmf_published_voter`` integrates it into the access decision of the publish
workflow.

This tag has the attribute ``priority``. The lower the priority number, the
earlier the voter gets to vote.

.. _`synchronized service`: https://symfony.com/doc/current/cookbook/service_container/scopes.html#a-using-a-synchronized-service
