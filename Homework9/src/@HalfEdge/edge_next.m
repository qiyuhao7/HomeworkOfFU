function retval = edge_next(HEObj,varargin)

if isempty(varargin) || isempty(varargin{1})
    e = 1:noe(HEObj);
else
    e = varargin{1};
end;

retval = double(HEObj.EN(e));


