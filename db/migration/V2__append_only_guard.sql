create or replace function forbid_audit_event_mutation()
returns trigger
language plpgsql
as $$
begin
    raise exception 'audit_events is append-only';
end;
$$;

create trigger audit_events_forbid_update
before update on audit_events
for each row
execute function forbid_audit_event_mutation();

create trigger audit_events_forbid_delete
before delete on audit_events
for each row
execute function forbid_audit_event_mutation();

