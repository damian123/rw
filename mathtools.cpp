double computeRV( const vector<double>&  path, 
                  const int              dayCount, 
                  const bool&            driftless )
{
    // compute cumulative variance
    double cum_var = 0.0, 
           r       = 0.0;
    
    size_t n = path.size();
    if( n <= 2 )
        THROW_ERROR("computeRV - invalid data size : " << n);

    if( path[0] <= ddr_tiny )
        THROW_ERROR("computeRV - invalid spot at data rank " << 1 << " level found is " << path[0] );
    
    for(size_t i=1; i<n; ++i)
    {
        if( path[i] <= ddr_tiny )
            THROW_ERROR("computeRV - invalid spot at data rank " << i+1 << " level found is " << path[i] );

        r        = log(path[i]/path[i-1]);
        cum_var += r * r;
    }


    // check day count
    if( dayCount <= 1 )
        THROW_ERROR("computeRV - invalid daycount: " << dayCount );
    
    double res = 0.0;
    if( driftless )
    {
        res = double(dayCount) * cum_var / double(n-1);
    }
    else
    {
        r   = log(path[n-1]/path[0]);
        res = double(dayCount) *  (cum_var - r * r / double(n-1)) / double(n-2); 
    }

    if( res <= -ddr_tiny )
        THROW_ERROR("computeRV - internal error : negative variance estimated: " << res);

    return sqrt(res);
}

class ComputeRVFn : public Function
{
    ComputeRVFn() : Function("ComputeRV", "compute the realized volatility along a spot path") {}
    static const ComputeRVFn m_instance;

    virtual void getArguments(vector<Argument> &ret) const
    {
        ret.push_back(Argument("sample",     ArgumentType::DOUBLE_ARRAY,     "spot path"));
        ret.push_back(Argument("dayCount",   ArgumentType::INTEGER,          "daycount", true, 252));
        ret.push_back(Argument("driftless",  ArgumentType::BOOL,             "type of variance : with/without drift", true, true));
    }
	
    virtual void eval(const ArgDictionary &args, Results &ret) const
    {
        // read inputs
		vecDOUBLE sample    = args.readDoubleVector("sample");
        int       dayCount   = args.readInt("dayCount");
        bool      driftless = args.readBool("driftless");
        
        // estimate historical volatility
        double res = computeRV(sample, dayCount, driftless);        
        ret.returnDouble(res);
    }
};
const ComputeRVFn ComputeRVFn::m_instance;
