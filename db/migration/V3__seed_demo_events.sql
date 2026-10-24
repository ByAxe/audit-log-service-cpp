insert into audit_events
    (id, recorded_at, actor, action, resource_type, resource_id, outcome, reason, metadata)
values
    (1, '2026-04-20T09:00:00Z', 'alice@example.com', 'LOGIN', 'session', 's-1001', 'ALLOWED', 'interactive login', '{"ip":"10.0.0.1"}'),
    (2, '2026-04-20T09:05:00Z', 'bob@example.com', 'ACCESS_PII', 'customer', 'c-2001', 'DENIED', 'missing approval', '{"ticket":"INC-101"}'),
    (3, '2026-04-20T09:10:00Z', 'carol@example.com', 'EXPORT_DATA', 'report', 'r-3001', 'ALLOWED', 'quarterly export', '{"format":"csv"}'),
    (4, '2026-04-20T09:15:00Z', 'alice@example.com', 'ACCESS_PII', 'customer', 'c-2002', 'ALLOWED', 'support case', '{"ticket":"SUP-44"}'),
    (5, '2026-04-20T09:20:00Z', 'dave@example.com', 'DELETE_RECORD', 'customer', 'c-2003', 'DENIED', 'destructive action blocked', '{"policy":"retention"}'),
    (6, '2026-04-20T09:25:00Z', 'erin@example.com', 'GRANT_ROLE', 'user', 'u-5001', 'ALLOWED', 'admin grant', '{"role":"auditor"}'),
    (7, '2026-04-20T09:30:00Z', 'bob@example.com', 'EXPORT_DATA', 'report', 'r-3002', 'DENIED', 'export limit exceeded', '{"format":"json"}'),
    (8, '2026-04-20T09:35:00Z', 'alice@example.com', 'LOGOUT', 'session', 's-1001', 'ALLOWED', 'session closed', '{"ip":"10.0.0.1"}'),
    (9, '2026-04-20T09:40:00Z', 'frank@example.com', 'REVOKE_ROLE', 'user', 'u-5002', 'ALLOWED', 'role cleanup', '{"role":"admin"}'),
    (10, '2026-04-20T09:45:00Z', 'carol@example.com', 'ACCESS_PII', 'customer', 'c-2004', 'DENIED', 'break-glass expired', '{"ticket":"SEC-9"}'),
    (11, '2026-04-20T09:50:00Z', 'gina@example.com', 'LOGIN', 'session', 's-1002', 'DENIED', 'mfa failed', '{"ip":"10.0.0.2"}'),
    (12, '2026-04-20T09:55:00Z', 'alice@example.com', 'EXPORT_DATA', 'report', 'r-3003', 'ALLOWED', 'case export', '{"format":"csv"}');

select setval('audit_events_id_seq', 12);
